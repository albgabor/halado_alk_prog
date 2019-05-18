#include <iostream>
#include "matrix.hh"
#include <random>
#include <chrono>
#include <fstream>
#include <string>

template<typename distrUOne, typename gen>
void testMatrixMult(int numOfNewThreads, std::string fileName, distrUOne distr, gen & gen, int nMin=25, int nMax=601){
    std::chrono::high_resolution_clock my_clock;

    double dt;
    int n=nMin;
    std::ofstream rs(fileName);
    rs << std::fixed;

    if (numOfNewThreads==0)
    {

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
    } while (n<nMax);

    } else if(numOfNewThreads==-1) {
    
    do
    {
        dt=1e+10;
        for (int k=0; k<100; ++k){
            Matrix<double> m1{n, n, [&](int r, int h){return distr(gen);}};
            Matrix<double> m2{n, n, [&](int r, int h){return distr(gen);}};
            auto t1 = my_clock.now();
            Matrix<double> res=multiplicationMultithread(m1, m2, m1.N);
            auto t2 = my_clock.now();
            double dt_this=(static_cast<std::chrono::duration<double, std::milli>>(t2-t1)).count();
            if (dt_this<dt) {dt=dt_this;}
        }
        std::cout << n << '\n';
        rs << n << '\t' << dt << '\n';
        n+=25;
    } while (n<nMax);

    } else {

    do
    {
        dt=1e+10;
        for (int k=0; k<100; ++k){
            Matrix<double> m1{n, n, [&](int r, int h){return distr(gen);}};
            Matrix<double> m2{n, n, [&](int r, int h){return distr(gen);}};
            auto t1 = my_clock.now();
            Matrix<double> res=multiplicationMultithread(m1, m2, numOfNewThreads);
            auto t2 = my_clock.now();
            double dt_this=(static_cast<std::chrono::duration<double, std::milli>>(t2-t1)).count();
            if (dt_this<dt) {dt=dt_this;}
        }
        std::cout << n << '\n';
        rs << n << '\t' << dt << '\n';
        n+=25;
    } while (n<nMax);

    }

}

int main(int, char**) {
    //std::random_device rd{};
    std::mt19937_64 gen(21);
    std::uniform_real_distribution<double> distr(0.0, 1.0);

    //Run with simple operator*
    testMatrixMult(0, "result_simple.txt", distr, gen);
    //Run on (1,2,4) new thread(s)
    testMatrixMult(1, "result_1.txt", distr, gen);
    testMatrixMult(2, "result_2.txt", distr, gen);
    testMatrixMult(4, "result_4.txt", distr, gen);
    //Run every row (of result) on different node
    testMatrixMult(-1, "result_many.txt", distr, gen);    
}
