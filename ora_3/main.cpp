#include <iostream>
#include <cmath>

template<typename F1, typename F2, typename T>
T Newton(F1 func, F2 derivative, T start, int steps=20) {
    T x=start;
    for (int k=0; k<steps; k++){
        x=x-func(x)/derivative(x);
    }
    return x;
}



int main(int, char**) {
    std::cout << "Hello, world!\n";

    std::cout << Newton([](double x){ return x*x-612.0;   },[](double x){ return 2.0*x; }, 10.0) << "\n";

}
