#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <algorithm>
#include <numeric>

const double d_inf=std::numeric_limits<double>::infinity();

//Simple function, that fits a line (a*x+b) to the datapoints, and returns with an array [a, b].
//(Two element array, the first element is 'a', the second is 'b'.)
std::array<double, 2> fit_line(const std::vector<double>& x_vals,
                               const std::vector<double>& y_vals,
                               const bool do_checks=true) {

    int x_size=x_vals.size();
    int y_size=y_vals.size();

    if (do_checks) {
        //Checks if there are enough elements in vectors.
        if (x_size<2 || y_size<2){
            std::cout << "Not enough element in arrays!\n\n";
            return std::array<double, 2> {d_inf, d_inf};
        //Checks if the size of vectors are the same.
        } else if (x_size!=y_size) {
            std::cout << "Sizes of the two vectors are not equal!\n\n";
            return std::array<double, 2> {d_inf, d_inf};
        //Checks if not all the elements are the same in 'x'.
        } else if (std::count(x_vals.begin(), x_vals.end(), x_vals.front())==x_size) {
            std::cout << "All elements of 'x' are the same.\n\n";
            return std::array<double, 2> {d_inf, d_inf};
        } else {
            std::cout << "All cheks passed.\n\n";
        }
    }
    
    double x_avg=std::accumulate(x_vals.begin(), x_vals.end(), 0.0)/x_size;
    double y_avg=std::accumulate(y_vals.begin(), y_vals.end(), 0.0)/y_size;
    
    double xy=std::inner_product(x_vals.begin(), x_vals.end(), y_vals.begin(), 0.0,
                                 [&](const double& acc, const double& product){return acc+product;},
                                 [&](const double& x, const double& y){return (x-x_avg)*(y-y_avg);});
    
    double xx=std::accumulate(x_vals.begin(), x_vals.end(), 0.0,
                              [&](const double& acc, const double& x){return acc+pow((x-x_avg), 2);});
    
    double a=xy/xx;
    
    return std::array<double, 2> {a, y_avg-a*x_avg};
}

int main()
{
    const std::vector<double> x={3.5, 5.1, 7.2, 9.8, 10.3};
    const std::vector<double> y={8.2, 11.0, 15.3, 20.9, 21.2}; //2*x+1+epsilon
//Examples for bad inputs
    //const std::vector<double> x={3.5, 5.1, 7.2, 9.8, 10.3};
    //const std::vector<double> y={8.2, 11.0, 15.3, 20.9};
    //-----------------------------------------------------
    //const std::vector<double> x={3.5};
    //const std::vector<double> y={8.2};
    //-----------------------------------------------------
    //const std::vector<double> x={3.5, 3.5, 3.5, 3.5, 3.5};
    //const std::vector<double> y={8.2, 11.0, 15.3, 20.9, 21.2};
    
    
    //fitted parameters via gnuplot
    const double a_gp=1.97725426614733;
    const double b_gp=1.12331436781433;
    
    
    std::array<double, 2> fit = fit_line(x, y);
    
    if (std::isinf(fit[0]) || std::isinf(fit[1])) {
        std::cout << "Fit didn't succeed!";
        return 1;
    } else {
        std::cout.precision(15);
        std::cout << "a_fit= " << fit[0] << "\tb_fit= " << fit[1] << '\n'
                  << "a_ref= " << a_gp << "\tb_ref= " << b_gp << '\n'
                  << "a_err= " << 100*(a_gp-fit[0])/a_gp << "%\tb_err= " << 100*(b_gp-fit[1])/b_gp << "%\n"
                  << "(err=100*(ref-fit)/ref %)\n\n";
        return 0;
    }              
}
