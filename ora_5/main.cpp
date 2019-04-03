#include "vector2.h"

int main(int, char**) {
    
	auto err = [](auto str){ std::cout << "vector2.h error in: " << str << "\n"; std::exit(-1); };

	//Test list initialization:
	{
		Vector2<double> a = {3.1, 5.2};
		if(a.x != 3.1){ err("initializer list constructor test [x]");   }
		if(a.y != 5.2){ err("initializer list constructor test [y]");   }
	}

	//Test copy constructor:
	{
		Vector2<double> a = {3.1, 5.2};
		Vector2<double> b{a};
		if(a.x != 3.1){ err("copy constructor test [src x]");   }
		if(a.y != 5.2){ err("copy constructor test [src y]");   }
		if(b.x != 3.1){ err("copy constructor test [x]");       }
		if(b.y != 5.2){ err("copy constructor test [y]");       }
	}
	
	//Test assignment:
	{
		Vector2<double> a = {3.1, 5.2};
		Vector2<double> b;
		b = a;
		if(a.x != 3.1){ err("assignment test [src x]");   }
		if(a.y != 5.2){ err("assignment test [src y]");   }
		if(b.x != 3.1){ err("assignment test [x]");       }
		if(b.y != 5.2){ err("assignment test [y]");       }
	}

	//Test self assignment:
	{
		Vector2<double> c = {3.1, 5.2};
		c = c;
		if(c.x != 3.1){ err("self assignment test [x]");       }
		if(c.y != 5.2){ err("self assignment test [y]");       }
	}

	//Test length, sqlength:
	{
		Vector2<double> c = {4.0, -3.0};
		double l = length(c);
		double sql = sqlength(c);
		if(std::abs(l - 5.0) > 1e-15)                 { err("length test [value]");        }
		if(std::abs(sql - 25.0) > 1e-15)              { err("sq length test [value]");     }
		if(c.x != 4.0){ err("length test [src x]");       }
		if(c.y != -3.0){ err("length test [src y]");       }
	}

	//Test operator-
	{
		Vector2<double> a   = {3.1, 5.2};
		Vector2<double> b   = {7.1, 15.2};
		Vector2<double> ref = {4.0, 10.0};
		Vector2<double> res = b - a;
		if(a.x != 3.1){ err("operator- test [src x]");       }
		if(a.y != 5.2){ err("operator- test [src y]");       }
		if(b.x != 7.1){ err("operator- test [src x]");       }
		if(b.y != 15.2){ err("operator- test [src y]");       }
		if(std::abs(ref.x-res.x) > 1e-15 ||
		   std::abs(ref.y-res.y) > 1e-15 ){ err("operator- test [elements]"); }
	}

	//Test operator+
	{
		Vector2<double> a   = {3.1, 5.2};
		Vector2<double> b   = {7.1, 15.2};
		Vector2<double> ref = {10.2, 20.4};
		Vector2<double> res = a + b;
		if(a.x != 3.1){ err("operator+ test [src x]");       }
		if(a.y != 5.2){ err("operator+ test [src y]");       }
		if(b.x != 7.1){ err("operator+ test [src x]");       }
		if(b.y != 15.2){ err("operator+ test [src y]");       }
		if(std::abs(ref.x-res.x) > 1e-15 ||
		   std::abs(ref.y-res.y) > 1e-15 ){ err("operator+ test [elements]"); }
	}

	//Test +=:
	{
		Vector2<double> a   = {3.1, 5.2};
		Vector2<double> b   = {7.1, 15.2};
		Vector2<double> ref = {10.2, 20.4};
	    a += b;
		if(b.x != 7.1){ err("+= test [src x]");       }
		if(b.y != 15.2){ err("+= test [src y]");       }
		if(std::abs(ref.x-a.x) > 1e-15 ||
		   std::abs(ref.y-a.y) > 1e-15 ){ err("+= test [elements]"); }
		
	}

	//Test -=:
	{
		Vector2<double> a   = {3.1, 5.2};
		Vector2<double> b   = {7.1, 15.2};
		Vector2<double> ref = {4.0, 10.0};
		b -= a;
		if(a.x != 3.1){ err("-= test [src x]");       }
		if(a.y != 5.2){ err("-= test [src y]");       }
		if(std::abs(ref.x-b.x) > 1e-15 ||
		   std::abs(ref.y-b.y) > 1e-15 ){ err("-= test [elements]"); }
	}

	//Test *=:
	{
		Vector2<double> a   = {3.1, -5.2};
		Vector2<double> ref = {6.2, -10.4};
		a *= 2.0;
		if(std::abs(ref.x-a.x) > 1e-15 ||
		   std::abs(ref.y-a.y) > 1e-15 ){ err("*= test [elements]"); }
	}

	//Test operator* (l-value, scalar)
	{
		Vector2<double> a   = {3.1, -5.2};
		Vector2<double> ref = {6.2, -10.4};
		Vector2<double> res = a * 2.0;
		if(a.x != 3.1){ err("operator* test (l-value, scalar) [src x]");       }
		if(a.y != -5.2){ err("operator* test (l-value, scalar) [src y]");       }
		if(std::abs(ref.x-res.x) > 1e-15 ||
		   std::abs(ref.y-res.y) > 1e-15 ){ err("operator* test (l-value, scalar) [elements]"); }
	}

	//Test operator* (scalar, l-value)
	{
		Vector2<double> a   = {3.1, -5.2};
		Vector2<double> ref = {6.2, -10.4};
		Vector2<double> res = 2.0 * a;
		if(a.x != 3.1){ err("operator* test (scalar, l-value) [src x]");       }
		if(a.y != -5.2){ err("operator* test (scalar, l-value) [src y]");       }
		if(std::abs(ref.x-res.x) > 1e-15 ||
		   std::abs(ref.y-res.y) > 1e-15 ){ err("operator* test (scalar, l-value) [elements]"); }
	}

	//Test /=:
	{
		Vector2<double> a   = {6.2, -10.4};
		Vector2<double> ref = {3.1, -5.2};
		a /= 2.0;
		if(std::abs(ref.x-a.x) > 1e-15 ||
		   std::abs(ref.y-a.y) > 1e-15 ){ err("/= test [elements]"); }
	}

	//Test operator/
	{
		Vector2<double> a   = {6.2, -10.4};
		Vector2<double> ref = {3.1, -5.2};
		Vector2<double> res = a / 2.0;
		if(a.x != 6.2){ err("operator/ test [src x]");       }
		if(a.y != -10.4){ err("operator/ test [src y]");       }
		if(std::abs(ref.x-res.x) > 1e-15 ||
		   std::abs(ref.y-res.y) > 1e-15 ){ err("operator/ test [elements]"); }
	}

	//Test dot:
	{
		Vector2<double> a = {3.1, -5.1};
		Vector2<double> b = {2.0, -3.0};
		double ref = 21.5;
		double res = dot(a, b);
		if(std::abs(ref-res) > 1e-15)                         { err("dot test [value]");        }
		if(a.x != 3.1){ err("dot test [src x]");       }
		if(a.y != -5.1){ err("dot test [src y]");       }
		if(b.x != 2.0){ err("dot test [src x]");       }
		if(b.y != -3.0){ err("dot test [src y]");       }
	}

	//Test normalize
	{
		Vector2<double> a   = {3.0, 4.0};
		Vector2<double> ref = {0.6, 0.8};
		Vector2<double> res = normalize(a);
		if(a.x != 3.0){ err("normalize test [src x]");       }
		if(a.y != 4.0){ err("normalize test [src y]");       }
		if(std::abs(ref.x-res.x) > 1e-15 ||
		   std::abs(ref.y-res.y) > 1e-15 ){ err("normalize test [elements]"); }
	}


    //Test <<
    {
        Vector2<double> v={2.0, 3.0};
        std::cout << "\n---Test of '<<' operator---\n";
        std::cout << "v.x is: " << v.x << "\tv.y is: " << v.y << '\n';
        std::cout << "v is: " << v << '\n';
    }

    //Test >>
    {
        Vector2<double> v;
        std::cout << "\n---Test of '>>' operator---\n";
        std::cout << "Vector2<double> v=\n";
        std::cin >> v;
        std::cout << "v is: " << v << '\n';
    }



    std::cout << "\nAll tests are successful!\n";
    return 0;

}
