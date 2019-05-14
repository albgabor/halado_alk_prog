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

    int nMin=25;
    int nMax=601;

    //Run with simple operator*
    {   
    double dt;
    int n=nMin;
    std::ofstream rs("result_simple.txt");
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
    } while (n<nMax);
    }

    //Run on 1 node
    {   
    double dt;
    int n=nMin;
    std::ofstream rs("result_1.txt");
    rs << std::fixed;
    do
    {
        dt=1e+10;
        for (int k=0; k<100; ++k){
            Matrix<double> m1{n, n, [&](int r, int h){return distr(gen);}};
            Matrix<double> m2{n, n, [&](int r, int h){return distr(gen);}};
            auto t1 = my_clock.now();
            Matrix<double> res=multiplicationMultithread(m1, m2, 1);
            auto t2 = my_clock.now();
            double dt_this=(static_cast<std::chrono::duration<double, std::milli>>(t2-t1)).count();
            if (dt_this<dt) {dt=dt_this;}
        }
    std::cout << n << '\n';
    rs << n << '\t' << dt << '\n';
    n+=25;
    } while (n<nMax);
    }

    //Run on 2 nodes
    {   
    double dt;
    int n=nMin;
    std::ofstream rs("result_2.txt");
    rs << std::fixed;
    do
    {
        dt=1e+10;
        for (int k=0; k<100; ++k){
            Matrix<double> m1{n, n, [&](int r, int h){return distr(gen);}};
            Matrix<double> m2{n, n, [&](int r, int h){return distr(gen);}};
            auto t1 = my_clock.now();
            Matrix<double> res=multiplicationMultithread(m1, m2, 2);
            auto t2 = my_clock.now();
            double dt_this=(static_cast<std::chrono::duration<double, std::milli>>(t2-t1)).count();
            if (dt_this<dt) {dt=dt_this;}
        }
    std::cout << n << '\n';
    rs << n << '\t' << dt << '\n';
    n+=25;
    } while (n<nMax);
    }

    //Run on 4 node
    {   
    double dt;
    int n=nMin;
    std::ofstream rs("result_4.txt");
    rs << std::fixed;
    do
    {
        dt=1e+10;
        for (int k=0; k<100; ++k){
            Matrix<double> m1{n, n, [&](int r, int h){return distr(gen);}};
            Matrix<double> m2{n, n, [&](int r, int h){return distr(gen);}};
            auto t1 = my_clock.now();
            Matrix<double> res=multiplicationMultithread(m1, m2, 4);
            auto t2 = my_clock.now();
            double dt_this=(static_cast<std::chrono::duration<double, std::milli>>(t2-t1)).count();
            if (dt_this<dt) {dt=dt_this;}
        }
    std::cout << n << '\n';
    rs << n << '\t' << dt << '\n';
    n+=25;
    } while (n<nMax);
    }

    //Run every row (of result) on different node
    {   
    double dt;
    int n=nMin;
    std::ofstream rs("result_many.txt");
    rs << std::fixed;
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
    }
    
}
