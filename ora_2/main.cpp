#include <iostream>
#include <math.h>

void SolveLinear(double a, double b)
{
  if( a == 0.0 )
  {
    std::cout << "No solution\n";
  }
  else
  {
    std::cout << "The solution is: " << -b/a << "\n";
  }
}

void SolveQuadratic (double a, double b, double c) {
    if( (a == 0.0) ) {
        SolveLinear(b,c);
    }
    else if (double D=b*b-4*a*c; D < 0.0) 
    {
        std::cout << "No solution\n";
    }
    
    else {
        std::cout << "The solutions are: " << -b+std::sqrt(D) << "; " << -b-std::sqrt(D) << "\n";
    }

}


double sqrt_newton(double num, double x0){


}




int main(int, char**) {
    SolveQuadratic(-1.0, 2.0, 3.0);
    return 0;
}
