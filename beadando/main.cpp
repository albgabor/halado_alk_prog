#include <iostream>
#include "scatter.hh"
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>


double testCrossSection(double theta){
    return std::sin(theta);
}

// This will generate some "histograms" on std::cout (for fast check)
//  - The generated theta distribution with energies 0.1 MeV and 0.5 MeV
//  - The generated phi distribution (with energie 0.1 MeV)
void fastCheck(){
    std::mt19937_64 gen(21);
    std::uniform_real_distribution<double> distUOne(0.0, 1.0);

    Photon testPhoton;
    auto testCSCompton=[&](double t){return testPhoton.diffCSCompton(t);};

    int testNum=100'000;
    
    testPhoton.en=0.1;
    std::vector<Direction> testDV_01(testNum);
    for (int k=0; k<testNum; ++k){
        testDV_01[k]=scatterUniformPhi(testCSCompton, distUOne, gen, testCSCompton(0.0)*1.01);
    }

    testPhoton.en=0.5;
    std::vector<Direction> testDV_05(testNum);
    for (int k=0; k<testNum; ++k){
        testDV_05[k]=scatterUniformPhi(testCSCompton, distUOne, gen, testCSCompton(0.0)*1.01);
    }


    int histNum=30;
    
    std::vector<int> testHistTheta_01(histNum, 0);
    std::vector<int> testHistPhi_01(histNum, 0);
    for (int k=0; k<testNum; ++k){
        ++testHistTheta_01[int(testDV_01[k].theta/(Const::pi/histNum))];
        ++testHistPhi_01[int( (testDV_01[k].phi+Const::pi)/(2*Const::pi/histNum) )];
    }

    std::cout << "\nHist Theta (0.1 MeV):\n";
    for (int k=0; k<histNum; ++k){
        std::cout << "[";
        for (int l=0; l<testHistTheta_01[k]/200; ++l){
            std::cout << '=';
        }
        std::cout << "]\n";
    }

    std::vector<int> testHistTheta_05(histNum, 0);
    std::vector<int> testHistPhi_05(histNum, 0);
    for (int k=0; k<testNum; ++k){
        ++testHistTheta_05[int(testDV_05[k].theta/(Const::pi/histNum))];
        ++testHistPhi_05[int( (testDV_05[k].phi+Const::pi)/(2*Const::pi/histNum))];
    }

    std::cout << "\nHist Theta (0.5 MeV):\n";
    for (int k=0; k<histNum; ++k){
        std::cout << "[";
        for (int l=0; l<testHistTheta_05[k]/200; ++l){
            std::cout << '=';
        }
        std::cout << "] \n";
    }

    
    std::cout << "\nHist Phi (0.1 MeV):\n";
    for (int k=0; k<histNum; ++k){
        std::cout << "[";
        for (int l=0; l<testHistPhi_01[k]/500; ++l){
            std::cout << '=';
        }
        std::cout << "]\n";
    }
}

bool isInCircle(double circX, double circY, double R, double x, double y){
    if ( (sqr(x-circX)+sqr(y-circY))<sqr(R) ){
        return true;
    } else {
        return false;
    }
}

//see: https://sci-hub.tw/10.1007/s10967-015-4579-2
void pipeSimulation(int photonNum, std::string filename="energy_hist.dat", int savedOrderOfScat=9, int numOfChanels=2048, double widthOfChanels=0.0002){
    std::random_device rd{};
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> distUOne(0.0, 1.0);

    double const electronDensityFe=26*7.874e+6*Const::avogadroNum/55.845;
    double const electronDensityAir=(0.78*14/28.014+0.22*16/31.998)*0.0012e+6*Const::avogadroNum;

    double const initialPhotonEnergy=0.662;
    double const sourceDistance=0.34;
    double const innerRadius=0.024;
    double const outerRadius=0.03;
    double const detectorDistance=0.16;
    double const detectorRadius=0.0381;
    double const detectorTheta=Const::pi*2.0/3.0;
    //The phi angle of detector is 0 by convention.

    std::cout << 1/(electronDensityAir*Photon{0.0, 0.0, 0.0, {0.0, 0.0}, initialPhotonEnergy}.totCSCompton()) << " m (Atlagos szabad uthossz levegoben)\n";
    std::cout << 1/(electronDensityFe*Photon{0.0, 0.0, 0.0, {0.0, 0.0}, initialPhotonEnergy}.totCSCompton()) << " m (Atlagos szabad uthossz vasban)\n";

    std::vector<int> scatteredNTimes(savedOrderOfScat+2);
    std::vector<std::vector<int>> histEnergy(savedOrderOfScat+2, std::vector<int>(numOfChanels, 0));

    for (int k=0; k<photonNum; k++){
        //The given collimator geometry with a point source in it
        auto collimator=[](double t){ if (t<std::atan(0.025)){return 1.0;} else {return 0.0;} };
        Photon testPhoton{0.0, 0.0, -sourceDistance, scatterUniformPhi(collimator, distUOne, gen, 1.01*std::sin(0.025), 0.025), initialPhotonEnergy};
        auto testCSCompton=[&](double t){return testPhoton.diffCSCompton(t);};
    
        int scatterCount=0;

        //The intersection (closer to the source) of the photon's path, and the pipe's outer cylinder/surface.
        //  The cylinder eq.: x^2+(z-outerRadius)^2=outerRadius^2
        //  The line of photon's path (with 3D vectors): (0.0,0.0,-sourceDistance)+param*(testPhoton.dir.getX(), testPhoton.dir.getY(), testPhoton.dir.getZ())
        {
        double A=sqr(testPhoton.dir.getX())+sqr(testPhoton.dir.getZ());
        double B=-2*testPhoton.dir.getZ()*(sourceDistance+outerRadius);
        double C=sqr(sourceDistance)+2*sourceDistance*outerRadius;
        double param=( -B-std::sqrt(sqr(B)-4*A*C) )/(2*A);
        if (param < 0){std::cout << "Elrontottam (henger, egyenes metszet)\n";}
        testPhoton.updatePos(param); //Step the photon to the outer surface of the 
        }

        //The first step (probably into the pipe).
        testPhoton.updatePos(getStepLength(1/(testPhoton.totCSCompton()*electronDensityFe), distUOne, gen));
        
        //While in the pipe.
        while ( isInCircle(0.0, outerRadius, outerRadius, testPhoton.x, testPhoton.z) ) {
            //If not in the inner hole (but in the wall of the pipe), there's a scattering process.
            if( !isInCircle(0.0, outerRadius, innerRadius, testPhoton.x, testPhoton.z) ){
                Direction scatDir=scatterUniformPhi(testCSCompton, distUOne, gen, testCSCompton(0.0));
                testPhoton.en=testPhoton.eScatCompton(scatDir.theta);
                testPhoton.dir=scatDir.rotate(testPhoton.dir.theta, testPhoton.dir.phi);
                ++scatterCount;
            }
            //Step the photon again.
            testPhoton.updatePos(getStepLength(1/(testPhoton.totCSCompton()*electronDensityFe), distUOne, gen));
            
        }
        if (scatterCount>savedOrderOfScat){
            ++scatteredNTimes[savedOrderOfScat+1];
        } else {
            ++scatteredNTimes[scatterCount];
        }


        //Check if the leaving photon will hit the face of the detector, and fill the energy histogram(s).
        //  Find the crossing point of the (origo centered) sphere on wich (the circle of) the detector is, and check if it's in the angle realm of the detector's face.
        //  The "measured" energy is drawn from a Gaussian distribution with 2.5% deviation (the mean is equal to the photon's energy).
        {
        double B=2*(testPhoton.x*testPhoton.dir.getX()+testPhoton.y*testPhoton.dir.getY()+testPhoton.z*testPhoton.dir.getZ());
        //( The radius of the sphere is sqrt(detectorDistance^2+detectorRadius^2) )
        double C=sqr(testPhoton.x)+sqr(testPhoton.y)+sqr(testPhoton.z)-std::sqrt(sqr(detectorDistance)+sqr(detectorRadius));
        double param=-B+std::sqrt(sqr(B)-4*C)/2.0;

        if(Direction{0.0, 0.0}.setFromXYZ( testPhoton.x+param*testPhoton.dir.getX(),
                                           testPhoton.y+param*testPhoton.dir.getY(),
                                           testPhoton.z+param*testPhoton.dir.getZ() ).rotate(-detectorTheta, 0.0).theta < std::atan(detectorRadius/detectorDistance)){
            double energy=std::normal_distribution<double>{testPhoton.en, testPhoton.en*0.025}(gen);
            ++histEnergy[0][int(energy/widthOfChanels)];
            if (scatterCount>savedOrderOfScat){
                ++histEnergy[savedOrderOfScat+1][int(energy/widthOfChanels)];
            } else {
                ++histEnergy[scatterCount][int(energy/widthOfChanels)];
            }
        }
        }
    }
    
    std::cout << "Szórások száma\tElőfordulási gyakoriság (%)  (az összes fotonra)\n";
    for (int k=0; k<savedOrderOfScat+1; ++k){
        std::cout << k << '\t' << 100.0*scatteredNTimes[k]/photonNum << '\n';
    }
    std::cout << savedOrderOfScat << "+\t" << 100.0*scatteredNTimes[savedOrderOfScat+1]/photonNum << '\n';
    
   std::ofstream ofs(filename);
   ofs << "#energy (keV)\ttotal count\tcounts for increasing scattering numbers (from 1)\n";
   for (int k=0; k<numOfChanels; ++k){
       ofs << (0.5+k)*widthOfChanels*1e+3;
       for (int l=0; l<savedOrderOfScat+2; ++l){
       ofs << '\t' << histEnergy[l][k];
       }
       ofs << '\n';
   }
    ofs.close();

}

int main(int, char**) {
    std::random_device rd{};
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> distUOne(0.0, 1.0);
    
    //double const electronDensityWater=10*1.0e+6*Const::avogadroNum/18.0; // [1/m^3] (Z*massDensity*avogadroNum/molarMass)
    double const electronDensitySi=14*2.329e+6*Const::avogadroNum/28.085; // [1/m^3] (Z*massDensity*avogadroNum/molarMass)
    double const electronDensityGe=32*5.323e+6*Const::avogadroNum/72.630; // [1/m^3] (Z*massDensity*avogadroNum/molarMass)
    //double const electronDensityFe=26*7.874e+6*Const::avogadroNum/55.845;
    //double const electronDensityAir=(0.78*14/28.014+0.22*16/31.998)*0.0012e+6*Const::avogadroNum;
    //double const electronDensityNaI=64*7.874e+6*Const::avogadroNum/55.845;

    //fastCheck();

    //pipeSimulation(int(75e+6));
    
    //Scattering in one layer (see: https://sci-hub.tw/https://www.sciencedirect.com/science/article/pii/016890029090832Q)
    {

    int photonNum=1'000'000;
    double layerThickness=0.01; //m
    double initialEnergy=0.5; //MeV
    double electronDensitiy=electronDensitySi;

    int savedOrderOfScat=9;
    std::vector<int> scatteredNTimes(savedOrderOfScat+2);

    for (int k=0; k<photonNum; k++){
        Photon testPhoton{0.0, 0.0, 0.0, {0.0, 0.0}, initialEnergy};
        auto testCSCompton=[&](double t){return testPhoton.diffCSCompton(t);};
    
        int scatterCount=0;

        testPhoton.updatePos(getStepLength(1/(testPhoton.totCSCompton()*electronDensitySi), distUOne, gen));

        while ( (testPhoton.z < layerThickness) && (testPhoton.z > 0.0) ){
            Direction scatDir=scatterUniformPhi(testCSCompton, distUOne, gen, testCSCompton(0.0));
            testPhoton.en=testPhoton.eScatCompton(scatDir.theta);
            testPhoton.dir=scatDir.rotate(testPhoton.dir.theta, testPhoton.dir.phi);
            ++scatterCount;
            testPhoton.updatePos(getStepLength(1/(testPhoton.totCSCompton()*electronDensitySi), distUOne, gen));
        }

        if (scatterCount>savedOrderOfScat){
            ++scatteredNTimes[savedOrderOfScat+1];
        } else {
            ++scatteredNTimes[scatterCount];
        }
    }

    std::cout << "Szórások száma\tElőfordulási gyakoriság (%)  (az összes fotonra)\n";
    for (int k=0; k<savedOrderOfScat+1; ++k){
        std::cout << k << '\t' << 100.0*scatteredNTimes[k]/photonNum << '\n';
    }
    std::cout << savedOrderOfScat << "+\t" << 100.0*scatteredNTimes[savedOrderOfScat+1]/photonNum << '\n';
    
    }

}
