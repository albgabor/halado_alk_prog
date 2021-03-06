#include <cmath>
#include <vector>

/* Units
mass: kg
length: m
time: s
energy: MeV
angle: rad
*/

template<typename T>
T sqr (T x){
    return x*x;
}

//Integrate a positive 1D function numerrically
//  To test if the integral of the differential cross section is equal to the total cross section.
/*
template<typename F, typename DistUOne, typename Gen>
double integratePositive(F func, DistUOne dist, Gen & gen, double from, double to, double max, int testpoints){
        double x;
        double y;
        int count=0;
    for (int k=0; k<testpoints; ++k){
        x=from+dist(gen)*(to-from);
        y=dist(gen)*max;
        if(y < func(x)){++count;}
    }
    return double(count)/double(testpoints)*(to-from)*max;
}
*/

namespace Const {
    double const mElectron=0.51099895; //MeV/c^2
    double const pi = std::acos(-1);
    double const radElectron=2.81794032e-15; //m (classical electron radius)
    double const totCSThomson=8*pi*sqr(radElectron)/3; //m^2
    double const avogadroNum=6.02214076e+23;
}


//Polar coordinate angles
//  theta is the angle from (the positive side of) z axis (theta is normally in [0,pi])
//  phi   is the angle from (the positive side of) x axis (toward the positive side of y axis) (phi is normally in [-pi,pi])
struct Direction
{
    double theta;
    double phi;

    double getX(){return std::sin(theta)*std::cos(phi);}
    double getY(){return std::sin(theta)*std::sin(phi);}
    double getZ(){return std::cos(theta);}

    Direction & setFromXYZ(double x, double y, double z){
        theta=std::acos(z/std::sqrt(sqr(x)+sqr(y)+sqr(z)));
        phi=std::atan2(y,x);
        return *this;
    }

    //As it's only a vector, 2 angles are enought
    //  alpha: rotation angle around Y (first  rotation)
    //  beta:  rotation angle around Z (second rotation)
    Direction & rotate(double alpha, double beta){
        double x_init=getX();
        double y_init=getY();
        double z_init=getZ();

        //Rotation around Y
        double x_alpha=x_init*std::cos(alpha)+z_init*std::sin(alpha);
            // y_alpha=y_init;
        double z_alpha=z_init*std::cos(alpha)-x_init*std::sin(alpha);

        //Rotation around Z
        double x_beta=x_alpha*std::cos(beta)-y_init*std::sin(beta);
        double y_beta=y_init*std::cos(beta)+x_alpha*std::sin(beta);
            // z_beta=z_alpha
        
        return setFromXYZ(x_beta, y_beta, z_alpha);
    }

};

struct Photon
{
    //position
    double x;
    double y;
    double z;

    //direction
    Direction dir;
    //energy (in MeV)
    double en=1.0;

    //Update the position with stepLength, in the direction of dir
    void updatePos(double stepLength){
        x+=stepLength*dir.getX();
        y+=stepLength*dir.getY();
        z+=stepLength*dir.getZ();

    }

    //scattered photon energy
    double eScatCompton(double theta){
        return en/(1+en*(1-std::cos(theta))/Const::mElectron);
    }

    //differential cross section of Compton scattering
    double diffCSCompton(double theta){
        double eRatio=eScatCompton(theta)/en;
        return 0.5*sqr(Const::radElectron)*sqr(eRatio)*(eRatio+1.0/eRatio-sqr(std::sin(theta)));
    }
    //total c. s. of Compton scat.
    double totCSCompton(){
        double x=en/Const::mElectron;
        return 0.75*Const::totCSThomson*(
                                        (1+x)*( 2*x*(1+x)/(1+2*x)-std::log(1+2*x) )/(x*x*x)+
                                        std::log(1+2*x)/(2*x)-(1+3*x)/sqr(1+2*x)
                                        );
    }

};


//Draws a direction (change) corresponding to the given differential cross section (in theta). (Uniform in phi distribution)
template<typename DiffCS, typename DistUOne, typename Gen>
Direction scatterUniformPhi(DiffCS cS, DistUOne dist, Gen & gen, double max, double maxTheta=Const::pi){
    double theta=maxTheta*dist(gen);
    double test=dist(gen);
    while (test > std::sin(theta)*cS(theta)/max)
    {
        theta=Const::pi*dist(gen);
        test=dist(gen);
    }
    return {theta, 2*Const::pi*(dist(gen)-0.5)};
}


//The probability of step with length is: e^(-length/meanFreePath) -> length=-ln(distrUniform[0,1])*meanFreePath
template<typename DistUOne, typename Gen>
double getStepLength(double meanFreePath, DistUOne dist, Gen & gen){
    return -std::log(dist(gen))*meanFreePath;
}

//Some other possibility to get step length, i've tried both, and seemed to work the same.
/*
template<typename DistUOne, typename Gen>
double getStepLength(double meanFreePath, DistUOne dist, Gen & gen, double checkDistance=1e-5){
    double distance=checkDistance;
    while (dist(gen)<std::exp(-checkDistance/meanFreePath)){distance+=checkDistance;}
    //while (dist(gen)<(1-checkDistance/meanFreePath)){distance+=checkDistance;}
    return distance;
}
*/