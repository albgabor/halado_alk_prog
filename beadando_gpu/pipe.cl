#include "mwc64x.cl"

__constant double static const mElectron=0.51099895; //MeV/c^2
__constant double const pi = 3.1415926535897932384626433832795;
__constant double const radElectron=2.81794032e-15; //m (classical electron radius)
__constant double const totCSThomson=8*3.1415926535897932384626433832795*2.81794032e-15*2.81794032e-15/3.; //m^2
//double static const avogadroNum=6.02214076e+23;

double sqr (double x){
    return x*x;
}

double getX(double theta, double phi){return sin(theta)*cos(phi);}
double getY(double theta, double phi){return sin(theta)*sin(phi);}
double getZ(double theta)            {return cos(theta);}

void setFromXYZ(double * theta, double * phi, double x, double y, double z){
    *theta=acos(z/sqrt(sqr(x)+sqr(y)+sqr(z)));
    *phi=atan2(y,x);
}

//As it's only a vector, 2 angles are enought
//  alpha: rotation angle around Y (first  rotation)
//  beta:  rotation angle around Z (second rotation)
void rotateDir(double * theta, double * phi, double alpha, double beta){
    double x_init=getX(*theta, *phi);
    double y_init=getY(*theta, *phi);
    double z_init=getZ(*phi);

    //Rotation around Y
    double x_alpha=x_init*cos(alpha)+z_init*sin(alpha);
        // y_alpha=y_init;
    double z_alpha=z_init*cos(alpha)-x_init*sin(alpha);

    //Rotation around Z
    double x_beta=x_alpha*cos(beta)-y_init*sin(beta);
    double y_beta=y_init*cos(beta)+x_alpha*sin(beta);
        // z_beta=z_alpha
    
    setFromXYZ(theta, phi, x_beta, y_beta, z_alpha);
}


//Update the position with stepLength, in the direction of dir
void updatePos(__global double * x,__global  double * y,__global  double * z,
               double theta, double phi, double stepLength){
    (*x)+=stepLength*getX(theta, phi);
    (*y)+=stepLength*getY(theta, phi);
    (*z)+=stepLength*getZ(phi);

}

//scattered photon energy
double eScatCompton(double en, double theta){
    return en/(1+en*(1-cos(theta))/mElectron);
}

//differential cross section of Compton scattering
double diffCSCompton(double en, double theta){
    double eRatio=eScatCompton(en, theta)/en;
    return 0.5*sqr(radElectron)*sqr(eRatio)*(eRatio+1.0/eRatio-sqr(sin(theta)));
}
//total c. s. of Compton scat.
double totCSCompton(double en){
    double x=en/mElectron;
    return 0.75*totCSThomson*(
                                    (1+x)*( 2*x*(1+x)/(1+2*x)-log(1+2*x) )/(x*x*x)+
                                    log(1+2*x)/(2*x)-(1+3*x)/sqr(1+2*x)
                                    );
}

bool isInCircle(double circX, double circY, double R, double x, double y){
    if ( (sqr(x-circX)+sqr(y-circY))<sqr(R) ){
        return true;
    } else {
        return false;
    }
}

bool isScattered(double meanFreePath, double stepLength, mwc64x_state_t * state){
    double ufloat=(double)(MWC64X_NextUint(state))/pow(2., 32.);
    double p=1-exp(-stepLength/meanFreePath);
    if (ufloat<p)
        {return true;}
    else
        {return false;}
}


//perform a scattering, update the direction and energy
void scatterUniformPhi(__global double * theta,__global  double * phi,__global  double * en, mwc64x_state_t * state){
    double dTheta=pi*(double)(MWC64X_NextUint(state))/pow(2., 32.);
    double ufloat=(double)(MWC64X_NextUint(state))/pow(2., 32.);
    while (ufloat > sin(dTheta)*diffCSCompton(*en, dTheta)/diffCSCompton(*en, 0.0))
    {
        dTheta=pi*(double)(MWC64X_NextUint(state))/pow(2., 32.);
        ufloat=(double)(MWC64X_NextUint(state))/pow(2., 32.);
    }
    *en=eScatCompton(*en, dTheta);

    ufloat=(double)(MWC64X_NextUint(state))/pow(2., 32.);
    double dPhi=2*pi*(ufloat-0.5);
    rotateDir(&dTheta, &dPhi, *theta, *phi);
    *theta=dTheta;
    *phi=dPhi;
}

//follows 'size' number of photons, until they leave pipe (up to 10000 steps)
//(if a photon didn't leave the pipe, it's simulation will continoue in the next "round")
//the counter tells how many "rounds" were already simulated
__kernel void stepInPipe(double electronDensity, double outerRadius,
                         double innerRadius, double stepLength, int size, int counter,
                         __global double * x, __global double * y, __global double * z,
                         __global int * volume, __global double * theta, __global double * phi,
                         __global double * en, __global int * scatterCount, __global unsigned char * isKilled)
{
	mwc64x_state_t rng;
    int numberPerPhoton=1000000000;
    MWC64X_SeedStreams(&rng, counter*size*numberPerPhoton,
                             numberPerPhoton*size/get_global_size(0));

    int gid=get_global_id(0);
    
    if (!isKilled[gid]){
        for (int j=0; j<10000; ++j){
            //is in pipe wall
            //volume[gid]=3;
            if (volume[gid]==1) {
                updatePos(&x[gid], &y[gid], &z[gid],
                        theta[gid], phi[gid], stepLength);
                if(isScattered(1./(totCSCompton(en[gid])*electronDensity), stepLength, &rng)){
                    ++scatterCount[gid];
                    scatterUniformPhi(&theta[gid], &phi[gid], &en[gid], &rng);
                }
                //if still in pipe
                if ( isInCircle(0.0, outerRadius, outerRadius, x[gid], z[gid]) ){
                    //if is in the inside of pipe
                    if ( isInCircle(0.0, outerRadius, innerRadius, x[gid], z[gid]) )
                        {volume[gid]=2;}
                } else {volume[gid]=3;}
            //is inside pipe
            } else if (volume[gid]==2) {
                updatePos(&x[gid], &y[gid], &z[gid],
                        theta[gid], phi[gid], stepLength);
                //if still in pipe
                if ( isInCircle(0.0, outerRadius, outerRadius, x[gid], z[gid]) ){
                    //if is in the wall of pipe
                    if ( !isInCircle(0.0, outerRadius, innerRadius, x[gid], z[gid]) )
                        {volume[gid]=1;}
               }else {volume[gid]=3;}
            } else if (volume[gid]==3){
                break;
            } else {
                volume[gid]=1;
            }
        }
    }
}
