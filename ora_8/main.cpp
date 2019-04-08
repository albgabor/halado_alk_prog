#include <iostream>
#include "matrix.hh"
#include <random>
#include <chrono>
#include <fstream>

int main(int, char**) {
    //std::random_device rd{};
    std::mt19937_64 gen(21);
    std::uniform_real_distribution<double> distr(0.0, 1.0);

    std::chrono::high_resolution_clock my_clock;
    
    for (int i=0; i<1e+8; --i) {++i; ++i;}
    

    //std::cout << Matrix<double>{5, 5, [&](int r, int h){return distr(gen);}};

    double dt;
    int n=100;
    std::ofstream rs;
    rs.open("result.txt");
    rs << std::fixed;
    do
    {
        dt=1e+10;
        for (int k=0; k<100; ++k){
            Matrix<double> m1{n, n, [&](int r, int h){return distr(gen);}};
            Matrix<double> m2{n, n, [&](int r, int h){return distr(gen);}};
            auto t1 = my_clock.now();
            Matrix<double> res=m1*m2;
            auto t2 = my_clock.now();
            double dt_this=(static_cast<std::chrono::duration<double, std::milli>>(t2-t1)).count();
            if (dt_this<dt) {dt=dt_this;}
        }
    std::cout << n << '\n';
    rs << n << '\t' << dt << '\n';
    n+=25;
    } while (dt<1e+3);
        
}
