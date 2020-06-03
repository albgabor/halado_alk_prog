#include <iostream>
#include "scatter.hh"
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

//#include "opencl_device.hpp"
//#include "opencl_buffer.hpp"
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 100
#define CL_HPP_TARGET_OPENCL_VERSION 120
#include "CLEXT/CL/cl2.hpp"

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

template<typename DistUOne, typename Gen>
Photon initPhotonForPipe(const double sourceDistance, const double initialPhotonEnergy,
                         const double outerRadius, DistUOne distUOne, Gen & gen){
    auto collimator=[](double t){ if (t<std::atan(0.025)){return 1.0;} else {return 0.0;} };
    Photon p{0.0, 0.0, -sourceDistance, 0, scatterUniformPhi(collimator, distUOne, gen, 0.026, 0.026), initialPhotonEnergy};
    //The intersection (closer to the source) of the photon's path, and the pipe's outer cylinder/surface.
    //  The cylinder eq.: x^2+(z-outerRadius)^2=outerRadius^2
    //  The line of photon's path (with 3D vectors): (0.0,0.0,-sourceDistance)+param*(testPhoton.dir.getX(), testPhoton.dir.getY(), testPhoton.dir.getZ())
    {
    double A=sqr(p.dir.getX())+sqr(p.dir.getZ());
    double B=-2*p.dir.getZ()*(sourceDistance+outerRadius);
    double C=sqr(sourceDistance)+2*sourceDistance*outerRadius;
    double param=( -B-std::sqrt(sqr(B)-4*A*C) )/(2*A);
    if (param < 0){std::cout << "Elrontottam (henger, egyenes metszet)\n";}
    p.updatePos(param); //Step the photon to the outer surface of the
    }
    p.volume=1;
    return p;
}

template<typename DistUOne, typename Gen>
void initPhotonVector(std::vector<Photon>& photons, double sourceDistance, const double initialPhotonEnergy,
                      const double outerRadius, DistUOne distUOne, Gen & gen){
    for (Photon & p : photons)
        p=initPhotonForPipe(sourceDistance, initialPhotonEnergy,
                            outerRadius, distUOne, gen);
}
template<typename DistUOne, typename Gen>
void initVectorPhoton(VectorPhoton & photons, double sourceDistance, const double initialPhotonEnergy,
                      const double outerRadius, DistUOne distUOne, Gen & gen){
    for (int k=0; k<photons.size; ++k){
        photons.setElement(k, initPhotonForPipe(sourceDistance, initialPhotonEnergy,
                                                outerRadius, distUOne, gen));
    }
}


template<typename DistUOne, typename Gen>
void stepPhotonsInPipe(std::vector<Photon> & photons, DistUOne distUOne, Gen & gen, const double electronDrensity,
                       const double outerRadius, const double innerRadius, const double stepLength){
    for (Photon & p : photons){
        if (p.isKilled) continue;
        //is in pipe wall
        if (p.volume==1) {
            p.updatePos(stepLength);
            if(isScattered(1/(p.totCSCompton()*electronDrensity), stepLength, distUOne, gen)){
                ++p.scatterCount;
                auto testCSCompton=[&](double t){return p.diffCSCompton(t);};
                Direction scatDir=scatterUniformPhi(testCSCompton, distUOne, gen, testCSCompton(0.0));
                p.en=p.eScatCompton(scatDir.theta);
                p.dir=scatDir.rotate(p.dir.theta, p.dir.phi);
            }
            //if still in pipe
            if ( isInCircle(0.0, outerRadius, outerRadius, p.x, p.z) ){
                //if is in the inside of pipe
                if ( isInCircle(0.0, outerRadius, innerRadius, p.x, p.z) )
                    p.volume=2;
            } else {p.volume=3;}
        //is inside pipe
        } else if (p.volume==2) {
            p.updatePos(stepLength);
            //if still in pipe
            if ( isInCircle(0.0, outerRadius, outerRadius, p.x, p.z) )
                //if is in the wall of pipe
                if ( isInCircle(0.0, outerRadius, innerRadius, p.x, p.z) )
                    p.volume=1;
            else
                p.volume=3;
        //left pipe
        } else if (p.volume==3) {
            std::cout << "stepPhotonsInPipe called with photon not killed after pipe!!!\n";
        //not entered pipe
        } else if (p.volume==0) {
            std::cout << "stepPhotonsInPipe called with photon at source positinon!!!\n";
        //a
        } else {
            std::cout << "stepPhotonsInPipe called with photon in unknown volume!!!\n";
        }
    }
}
template<typename DistUOne, typename Gen>
void stepPhotonsInPipe(VectorPhoton & photons, DistUOne distUOne, Gen & gen, const double electronDrensity,
                       const double outerRadius, const double innerRadius, const double stepLength){
    for (int k=0; k<photons.size; ++k){
        if (photons.isKilled.at(k)) continue;
        //is in pipe wall
        for (int j=0; j<10000; ++j) {
            if (photons.volume.at(k)==1) {
                photons.updateElementPos(k, stepLength);
                if(isScattered(1/(photons.totCSComptonElement(k)*electronDrensity), stepLength, distUOne, gen)){
                    ++photons.scatterCount.at(k);
                    auto testCSCompton=[&](double t){return photons.diffCSComptonElement(k, t);};
                    Direction scatDir=scatterUniformPhi(testCSCompton, distUOne, gen, testCSCompton(0.0));
                    photons.en.at(k)=photons.eScatComptonElement(k, scatDir.theta);
                    photons.dir.setElement(k, scatDir.rotate(photons.dir.theta.at(k), photons.dir.phi.at(k)));
                }
                //if still in pipe
                if ( isInCircle(0.0, outerRadius, outerRadius, photons.x.at(k), photons.z.at(k)) ){
                    //if is in the inside of pipe
                    if ( isInCircle(0.0, outerRadius, innerRadius, photons.x.at(k), photons.z.at(k)) )
                        photons.volume.at(k)=2;
                } else {photons.volume.at(k)=3;}
            //is inside pipe
            } else if (photons.volume.at(k)==2) {
                photons.updateElementPos(k, stepLength);
                //if still in pipe
                if ( isInCircle(0.0, outerRadius, outerRadius, photons.x.at(k), photons.z.at(k)) ){
                    //if is in the wall of pipe
                    if ( !isInCircle(0.0, outerRadius, innerRadius, photons.x.at(k), photons.z.at(k)) )
                        photons.volume.at(k)=1;
                }else {photons.volume.at(k)=3;}
            //left pipe
            } else if (photons.volume.at(k)==3) {
                break;
            //not entered pipe
            //} else if (photons.volume.at(k)==0) {
            //    std::cout << "stepPhotonsInPipe called with photon at source positinon!!!\n";
            //a
            } else {
                //std::cout << "stepPhotonsInPipe called with photon in unknown volume!!!\n";
                photons.volume.at(k)=1;
            }
        }
    }
}

template<typename Gen>
void processPhotonsAfterPipe(std::vector<Photon> & photons, std::vector<std::vector<int>> & histEnergy, const double widthOfChanels, const int savedOrderOfScat,
                             const double detectorDistance, const double detectorRadius, const double detectorTheta, Gen & gen, int & processedPhotons){
    for (Photon & p : photons){
        if ( (p.volume!=3) || p.isKilled ) continue;
        //Check if the leaving photon will hit the face of the detector, and fill the energy histogram(s).
        //  Find the crossing point of the (origo centered) sphere on wich (the circle of) the detector is, and check if it's in the angle realm of the detector's face.
        //  The "measured" energy is drawn from a Gaussian distribution with 2.5% deviation (the mean is equal to the photon's energy).
        double B=2*(p.x*p.dir.getX()+p.y*p.dir.getY()+p.z*p.dir.getZ());
        //( The radius of the sphere is sqrt(detectorDistance^2+detectorRadius^2) )
        double C=sqr(p.x)+sqr(p.y)+sqr(p.z)-std::sqrt(sqr(detectorDistance)+sqr(detectorRadius));
        double param=-B+std::sqrt(sqr(B)-4*C)/2.0;

        if(Direction{0.0, 0.0}.setFromXYZ( p.x+param*p.dir.getX(),
                                           p.y+param*p.dir.getY(),
                                           p.z+param*p.dir.getZ() ).rotate(-detectorTheta, 0.0).theta < std::atan(detectorRadius/detectorDistance)){
            double energy=std::normal_distribution<double>{p.en, p.en*0.025}(gen);
            int index = int(energy/widthOfChanels);
            if ( (index>-1) && (index<histEnergy.at(0).size()) ){
                ++histEnergy.at(0).at(index);
                if (p.scatterCount>savedOrderOfScat){
                    ++histEnergy.at(savedOrderOfScat+1).at(index);
                } else {
                    ++histEnergy.at(p.scatterCount).at(index);
                }
            }
        }
        p.isKilled=true;
        ++processedPhotons;
    }
}

template<typename Gen>
void processPhotonsAfterPipe(VectorPhoton & photons, std::vector<std::vector<int>> & histEnergy, const double widthOfChanels, const int savedOrderOfScat,
                             const double detectorDistance, const double detectorRadius, const double detectorTheta, Gen & gen, int & processedPhotons){
    for (int k=0; k<photons.size; ++k){
        if ( (photons.volume.at(k)!=3) || photons.isKilled.at(k) ) continue;
        //Check if the leaving photon will hit the face of the detector, and fill the energy histogram(s).
        //  Find the crossing point of the (origo centered) sphere on wich (the circle of) the detector is, and check if it's in the angle realm of the detector's face.
        //  The "measured" energy is drawn from a Gaussian distribution with 2.5% deviation (the mean is equal to the photon's energy).
        double B=2*(photons.x.at(k)*photons.dir.getElementX(k)+photons.y.at(k)*photons.dir.getElementY(k)+photons.z.at(k)*photons.dir.getElementZ(k));
        //( The radius of the sphere is sqrt(detectorDistance^2+detectorRadius^2) )
        double C=sqr(photons.x.at(k))+sqr(photons.y.at(k))+sqr(photons.z.at(k))-std::sqrt(sqr(detectorDistance)+sqr(detectorRadius));
        double param=-B+std::sqrt(sqr(B)-4*C)/2.0;

        if(Direction{0.0, 0.0}.setFromXYZ( 
                                           photons.x.at(k)+param*photons.dir.getElementX(k),
                                           photons.y.at(k)+param*photons.dir.getElementY(k),
                                           photons.z.at(k)+param*photons.dir.getElementZ(k)
                                         ).rotate(-detectorTheta, 0.0).theta < std::atan(detectorRadius/detectorDistance)){
            double energy=std::normal_distribution<double>{photons.en.at(k), photons.en.at(k)*0.025}(gen);
            int index = int(energy/widthOfChanels);
            if ( (index>-1) && (index<histEnergy.at(0).size()) ){
                ++histEnergy.at(0).at(index);
                if (photons.scatterCount.at(k)>savedOrderOfScat){
                    ++histEnergy.at(savedOrderOfScat+1).at(index);
                } else {
                    ++histEnergy.at(photons.scatterCount.at(k)).at(index);
                }
            }
        }
        photons.isKilled.at(k)=(unsigned char)(true);
        ++processedPhotons;
    }
}

template<typename DistUOne, typename Gen>
void replaceKilledPhotons(std::vector<Photon> & photons, double sourceDistance, const double initialPhotonEnergy,
                      const double outerRadius, DistUOne distUOne, Gen & gen, int & createdPhotons, const int photonNum){
    for (Photon & p : photons){
        if (createdPhotons==photonNum) break;
        if (p.isKilled){
            p=initPhotonForPipe(sourceDistance, initialPhotonEnergy, outerRadius, distUOne, gen);
            ++createdPhotons;
        }
    }
}
template<typename DistUOne, typename Gen>
void replaceKilledPhotons(VectorPhoton & photons, double sourceDistance, const double initialPhotonEnergy,
                      const double outerRadius, DistUOne distUOne, Gen & gen, int & createdPhotons, const int photonNum){
    for (int k=0; k<photons.size; ++k){
        if (createdPhotons==photonNum) break;
        if (photons.isKilled.at(k)){
            photons.setElement(k, initPhotonForPipe(sourceDistance, initialPhotonEnergy,
                                                    outerRadius, distUOne, gen));
            ++createdPhotons;
        }
    }
}

//see: https://sci-hub.tw/10.1007/s10967-015-4579-2
int pipeSimulation(int photonNum, int vectorSize=10000, double stepLength=0.0003, std::string filename="energy_hist.dat", int savedOrderOfScat=9, int numOfChanels=512, double widthOfChanels=0.0008){
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

    std::cout << 1/(electronDensityAir*Photon{0.0, 0.0, 0.0, 0, {0.0, 0.0}, initialPhotonEnergy}.totCSCompton()) << " m (Atlagos szabad uthossz levegoben)\n";
    std::cout << 1/(electronDensityFe*Photon{0.0, 0.0, 0.0, 0, {0.0, 0.0}, initialPhotonEnergy}.totCSCompton()) << " m (Atlagos szabad uthossz vasban)\n";

    //std::vector<int> scatteredNTimes(savedOrderOfScat+2);
    std::vector<std::vector<int>> histEnergy(savedOrderOfScat+2, std::vector<int>(numOfChanels, 0));

    int createdPhotons=0;
    int processedPhotons=0;

    //std::vector<Photon> photons(vectorSize);
    VectorPhoton photons{vectorSize};
    initVectorPhoton(photons, sourceDistance, initialPhotonEnergy, outerRadius, distUOne, gen);
    createdPhotons=vectorSize;

        try{
            std::vector<cl::Platform> platforms;
		    cl::Platform::get(&platforms);
            if (platforms.empty()) throw std::exception{ "No OpenCL platforms found." };
            std::cout << "Found platform" << (platforms.size() > 1 ? "s" : "") << ":\n";
            for (const auto& platform : platforms)
                std::cout << "\t" << platform.getInfo<CL_PLATFORM_VENDOR>() << std::endl;
            //select platform, R7 370 for me
            cl::Platform plat = platforms.at(0);
            std::cout << "Selected platform: " << plat.getInfo<CL_PLATFORM_VENDOR>() << std::endl;

            std::vector<cl::Device> devices;
            plat.getDevices(CL_DEVICE_TYPE_ALL, &devices);
            std::cout << "Found device" << (devices.size() > 1 ? "s" : "") << ":\n";
            for (const auto& device : devices)
                std::cout << "\t" << device.getInfo<CL_DEVICE_NAME>() << std::endl;
            cl::Device device = devices.at(0);
            std::cout << "Selected device: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
            
            std::vector<cl_context_properties> props{ CL_CONTEXT_PLATFORM, reinterpret_cast<cl_context_properties>((plat)()), 0 };
            cl::Context context{ devices, props.data() };

            cl::CommandQueue queue{ context, device};

            // Load program source
            std::string const kernel_location{"pipe.cl"};
            std::ifstream source_file{ kernel_location };
            if (!source_file.is_open())
                throw std::runtime_error{ std::string{"Cannot open kernel source: "} + kernel_location };
            
            // Create program and kernel
            cl::Program program{ context, std::string{ std::istreambuf_iterator<char>{ source_file },
			                                       std::istreambuf_iterator<char>{} } };
            program.build({ device }, "-cl-std=CL1.2 -I./cl"); // Any warning counts as a compilation error, simplest kernel syntax
            
            cl::KernelFunctor k=cl::KernelFunctor<
                    double, double, double, double, int, int, cl::Buffer, cl::Buffer, cl::Buffer,
                    cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer, cl::Buffer
                    >(program, "stepInPipe");
            int counter=0;
            while (processedPhotons<photonNum){
                //without openCL
                stepPhotonsInPipe(photons, distUOne, gen, electronDensityFe, outerRadius, innerRadius, stepLength);

                //with openCL
                /*
                cl::Buffer buf_x{ queue, std::begin(photons.x), std::end(photons.x), false },
                        buf_y{ queue, std::begin(photons.y), std::end(photons.y), false },
                        buf_z{ queue, std::begin(photons.z), std::end(photons.z), false },
                        buf_volume{ queue, std::begin(photons.volume), std::end(photons.volume), false },
                        buf_theta{ queue, std::begin(photons.dir.theta), std::end(photons.dir.theta), false },
                        buf_phi{ queue, std::begin(photons.dir.phi), std::end(photons.dir.phi), false },
                        buf_en{ queue, std::begin(photons.en), std::end(photons.en), false },
                        buf_scatterCount{ queue, std::begin(photons.scatterCount), std::end(photons.scatterCount), false },
                        buf_isKilled{ queue, std::begin(photons.isKilled), std::end(photons.isKilled), false };
                
                cl::Event kernel_event{k(cl::EnqueueArgs{queue, cl::NDRange{cl::size_type(photons.size)}},
                                    electronDensityFe, outerRadius, innerRadius, stepLength,
                                    photons.size, counter,
                                    buf_x, buf_y, buf_z, buf_volume, buf_theta, buf_phi,
                                    buf_en, buf_scatterCount, buf_isKilled)};
                kernel_event.wait();
                cl::copy(queue, buf_x, std::begin(photons.x), std::end(photons.x));
                cl::copy(queue, buf_y, std::begin(photons.y), std::end(photons.y));
                cl::copy(queue, buf_z, std::begin(photons.z), std::end(photons.z));
                cl::copy(queue, buf_volume, std::begin(photons.volume), std::end(photons.volume));
                cl::copy(queue, buf_theta, std::begin(photons.dir.theta), std::end(photons.dir.theta));
                cl::copy(queue, buf_phi, std::begin(photons.dir.phi), std::end(photons.dir.phi));
                cl::copy(queue, buf_en, std::begin(photons.en), std::end(photons.en));
                cl::copy(queue, buf_scatterCount, std::begin(photons.scatterCount), std::end(photons.scatterCount));
                cl::copy(queue, buf_isKilled, std::begin(photons.isKilled), std::end(photons.isKilled));
                */

                processPhotonsAfterPipe(photons, histEnergy, widthOfChanels, savedOrderOfScat, detectorDistance, detectorRadius, detectorTheta, gen, processedPhotons);
                replaceKilledPhotons(photons, sourceDistance, initialPhotonEnergy, outerRadius, distUOne, gen, createdPhotons, photonNum);
                ++counter;
            }
        }  
        catch (cl::BuildError& error) // If kernel failed to build
        {
            std::cout << "Build ERROR!" << std::endl;
            std::cerr << error.what() << "(" << error.err() << ")" << std::endl;

            for (const auto& log : error.getBuildLog())
            {
                std::cerr <<
                    "\tBuild log for device: " <<
                    log.first.getInfo<CL_DEVICE_NAME>() <<
                    std::endl << std::endl <<
                    log.second <<
                    std::endl << std::endl;
            }

            std::exit(error.err());
        }
        catch (cl::Error& error) // If any OpenCL error occurs
        {
            std::cout << "CL ERROR!" << std::endl;
            std::cerr << error.what() << "(" << error.err() << ")" << std::endl;

            std::exit(error.err());
        }
        catch (std::exception& error) // If STL/CRT error occurs
        {
            std::cout << "STD ERROR!" << std::endl;
            std::cerr << error.what() << std::endl;

            std::exit(EXIT_FAILURE);
        }
    
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
    return 0;
}

int main(int, char**) {
    std::random_device rd{};
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<double> distUOne(0.0, 1.0);
    
    //double const electronDensityWater=10*1.0e+6*Const::avogadroNum/18.0; // [1/m^3] (Z*massDensity*avogadroNum/molarMass)
    //double const electronDensitySi=14*2.329e+6*Const::avogadroNum/28.085; // [1/m^3] (Z*massDensity*avogadroNum/molarMass)
    //double const electronDensityGe=32*5.323e+6*Const::avogadroNum/72.630; // [1/m^3] (Z*massDensity*avogadroNum/molarMass)
    //double const electronDensityFe=26*7.874e+6*Const::avogadroNum/55.845;
    //double const electronDensityAir=(0.78*14/28.014+0.22*16/31.998)*0.0012e+6*Const::avogadroNum;
    //double const electronDensityNaI=64*7.874e+6*Const::avogadroNum/55.845;

    //fastCheck();

    pipeSimulation(int(1e+7), int(1e+5));    
}
