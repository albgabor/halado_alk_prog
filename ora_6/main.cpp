#include <iostream>
#include "matrix.hh"

int main(int, char**) {
  auto err = [](auto str){ std::cout << "matrix.hh error in: " << str << "\n"; std::exit(-1); };
	
	Matrix<double> m1={2, 2, {1.0, 2.0, 3.0, 4.0}};
	//std::cout << m1.data.size() <<m1.N << m1.M;

	//Test default constructor:
	{
	Matrix<double> u;
	if(u.data.size() != 0)         { err("default contructor test [size]");           }
	if(u.data.begin() != u.data.end())  { err("default contructor test [begin == end]");   }
	if(u.data.cbegin() != u.data.cend()){ err("default contructor test [cbegin == cend]"); }
  }

	//Test Matrix initialization and indexing:
	{
		Matrix<double> a = {2, 3, {3.1, 5.2, 9.3, 4.1, 2.5, 1.3}};
		if(a.data.size() != 6)                            { err("initializer list constructor test [size]");               }
		if(a.N != 2)                            { err("initializer list constructor test [N]");               }
		if(a.M != 3)                            { err("initializer list constructor test [M]");               }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(0, 2) != 9.3 ||
			 a(1, 0) != 4.1 || a(1, 1) != 2.5 || a(1, 2) != 1.3) { err("initializer list constructor test [indexing]");   }
		if( ++ ++ ++ ++ ++ ++(a.data.begin()) != a.data.end() )         { err("initializer list constructor test [begin + 6 == end]");   }
		if( ++ ++ ++ ++ ++ ++(a.data.cbegin()) != a.data.cend() )       { err("initializer list constructor test [cbegin + 6 == cend]"); }
	}

	//Test copy constructor:
	{
		Matrix<double> a = {2, 3, {3.1, 5.2, 9.3, 4.1, 2.5, 1.3}};
		Matrix<double> b{a};
		if(b.data.size() != 6)                            { err("copy constructor test [size]");               }
		if(b(0, 0) != 3.1 || b(0, 1) != 5.2 || b(0, 2) != 9.3 ||
			 b(1, 0) != 4.1 || b(1, 1) != 2.5 || b(1, 2) != 1.3) { err("copy constructor test [indexing]");   }
		if( ++ ++ ++ ++ ++ ++(b.data.begin()) != b.data.end() )         { err("copy constructor test [begin + 6 == end]");   }
		if( ++ ++ ++ ++ ++ ++(b.data.cbegin()) != b.data.cend() )       { err("copy constructor test [cbegin + 6 == cend]"); }
		if(a.data.size() != 6)                            { err("copy constructor test [src size]");           }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(0, 2) != 9.3 ||
			 a(1, 0) != 4.1 || a(1, 1) != 2.5 || a(1, 2) != 1.3) { err("copy constructor test [src elements]");       }
	}

	//Test move constructor:
	{
		Matrix<double> a = {2, 3, {3.1, 5.2, 9.3, 4.1, 2.5, 1.3}};
		Matrix<double> b{ std::move(a) };
		if(a.data.size() != 0)                            { err("move constructor test [src size]");             }
//		if(a.N != 0)                            { err("move constructor test [N]");             }  //the default move constructor don't set it to zero
//		if(a.M != 0)                            { err("move constructor test [M]");             }  //the default move constructor don't set it to zero
		if(a.data.begin() != a.data.end())                     { err("move constructor test [src begin == src end]"); }
		if(b.data.size() != 6)                            { err("move constructor test [size]");                 }
		if(b(0, 0) != 3.1 || b(0, 1) != 5.2 || b(0, 2) != 9.3 ||
			 b(1, 0) != 4.1 || b(1, 1) != 2.5 || b(1, 2) != 1.3) { err("move constructor test [indexing]");   }
		if( ++ ++ ++ ++ ++ ++(b.data.begin()) != b.data.end() )         { err("move constructor test [begin + 6 == end]");   }
		if( ++ ++ ++ ++ ++ ++(b.data.cbegin()) != b.data.cend() )       { err("move constructor test [cbegin + 6 == cend]"); }
	}

		//Test assignment:
	{
		Matrix<double> a = {2, 3, {3.1, 5.2, 9.3, 4.1, 2.5, 1.3}};
		Matrix<double> b;
		b = a;
		if(b.data.size() != 6)                            { err("assignment test [size]");         }
		if(b.N != 2)                            { err("assignment test [N]");             }
		if(b.M != 3)                            { err("assignment test [M]");             }
		if(b(0, 0) != 3.1 || b(0, 1) != 5.2 || b(0, 2) != 9.3 ||
			 b(1, 0) != 4.1 || b(1, 1) != 2.5 || b(1, 2) != 1.3) { err("assignment test [indexing]");   }
		if(a.data.size() != 6)                            { err("assignment test [src size]");     }
		if(a.N != 2)                            { err("assignment test [N]");             }
		if(a.M != 3)                            { err("assignment test [M]");             }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(0, 2) != 9.3 ||
			 a(1, 0) != 4.1 || a(1, 1) != 2.5 || a(1, 2) != 1.3) { err("assignment test [src elements]");       }
	}

	//Test self assignment:
	{
		Matrix<double> c = {2, 3, {3.1, 5.2, 9.3, 4.1, 2.5, 1.3}};
		c = c;
		if(c.data.size() != 6)                            { err("self assignment test [size]");     }
		if(c(0, 0) != 3.1 || c(0, 1) != 5.2 || c(0, 2) != 9.3 ||
			 c(1, 0) != 4.1 || c(1, 1) != 2.5 || c(1, 2) != 1.3) { err("self assignment test [elements]");       }
	}

	//Test move assignment:
	{
		Matrix<double> a = {2, 3, {3.1, 5.2, 9.3, 4.1, 2.5, 1.3}};
		Matrix<double> b;
		b = std::move(a);
		if(a.data.begin() != a.data.end())                     { err("move assignment test [src begin == src end]"); }
		if(a.data.size() != 0)                            { err("move assignment test [src size]");             }
//		if(a.N != 0)                            { err("move assignment test [N]");             }  //the default move constructor don't set it to zero
//		if(a.M != 0)                            { err("move assignment test [M]");             }  //the default move constructor don't set it to zero
		if(b.data.size() != 6)                            { err("move assignment test [size]");                 }
		if(b(0, 0) != 3.1 || b(0, 1) != 5.2 || b(0, 2) != 9.3 ||
			 b(1, 0) != 4.1 || b(1, 1) != 2.5 || b(1, 2) != 1.3) { err("move assignment test [indexing]");   }
		if( ++ ++ ++ ++ ++ ++(b.data.begin()) != b.data.end() )         { err("move assignment test [begin + 6 == end]");   }
		if( ++ ++ ++ ++ ++ ++(b.data.cbegin()) != b.data.cend() )       { err("move assignment test [cbegin + 6 == cend]"); }
	}

	//Test self move assignment:
	{
		Matrix<double> c = {2, 3, {3.1, 5.2, 9.3, 4.1, 2.5, 1.3}};
		c = std::move(c);
		if(c.data.size() != 6)                            { err("self move assignment test [size]");     }
		if(c(0, 0) != 3.1 || c(0, 1) != 5.2 || c(0, 2) != 9.3 ||
			 c(1, 0) != 4.1 || c(1, 1) != 2.5 || c(1, 2) != 1.3) { err("self move assignment test [indexing]");   }
	}

	//Test operator- (l-value, l-value) (more detailed test, since later tests depend on it)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2,2,{4.0, 10.0, 2.0, -1.0}};
		Matrix<double> res = b - a;
		if(a.data.size() != 4)                              { err("operator- test (l-value, l-value) [src size]");     }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1)  { err("operator- test (l-value, l-value) [src elements]"); }
		if(b.data.size() != 4)                              { err("operator- test (l-value, l-value) [src size]");     }
		if(b(0, 0) != 7.1 || b(0, 1) != 15.2 || b(1, 0) != 11.3 || b(1, 1) != 3.1){ err("operator- test (l-value, l-value) [src elements]"); }
		if(res.data.size() != 4)                            { err("operator- test (l-value, l-value) [size]");         }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   ) { err("operator- test (l-value, l-value) [elements]"); }
	}

	//Test operator- (r-value, l-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2,2,{4.0, 10.0, 2.0, -1.0}};
		Matrix<double> res = std::move(b) - a;
		if(a.data.size() != 4)                              { err("operator- test (r-value, l-value) [src size]");     }
		if(b.data.size() != 0)                              { err("operator- test (r-value, l-value) [src size]");     }
		if(res.data.size() != 4)                            { err("operator- test (r-value, l-value) [size]");         }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1){ err("operator- test (r-value, l-value) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )                  { err("operator- test (r-value, l-value) [value]");        }
	}

	//Test operator- (l-value, r-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2,2,{4.0, 10.0, 2.0, -1.0}};
		Matrix<double> res = b - std::move(a);
		if(a.data.size() != 0)                              { err("operator- test (l-value, r-value) [src size]");     }
		if(b.data.size() != 4)                              { err("operator- test (l-value, r-value) [src size]");     }
		if(res.data.size() != 4)                            { err("operator- test (l-value, r-value) [size]");         }
		if(b(0, 0) != 7.1 || b(0, 1) != 15.2 || b(1, 0) != 11.3 || b(1, 1) != 3.1){ err("operator- test (l-value, r-value) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )                  { err("operator- test (l-value, r-value) [value]");        }
	}

	//Test operator- (r-value, r-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2,2,{4.0, 10.0, 2.0, -1.0}};
		Matrix<double> res = std::move(b) - std::move(a);
		if(a.data.size() != 4)            { err("operator- test (r-value, r-value) [src size]"); }//this argument was not reused!
		if(b.data.size() != 0)            { err("operator- test (r-value, r-value) [src size]"); }
		if(res.data.size() != 4)          { err("operator- test (r-value, r-value) [size]");     }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   ){ err("operator- test (r-value, r-value) [value]");    }
	}

	//Test operator+ (l-value, l-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2, 2, {10.2, 20.4, 20.6, 7.2}};
		Matrix<double> res = a + b;
		if(a.data.size() != 4)                              { err("operator+ test (l-value, l-value) [src size]");     }
		if(b.data.size() != 4)                              { err("operator+ test (l-value, l-value) [src size]");     }
		if(res.data.size() != 4)                            { err("operator+ test (l-value, l-value) [size]");         }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1){ err("operator+ test (l-value, l-value) [src elements]"); }
		if(b(0, 0) != 7.1 || b(0, 1) != 15.2 || b(1, 0) != 11.3 || b(1, 1) != 3.1){ err("operator+ test (l-value, l-value) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )         { err("operator+ test (l-value, l-value) [value]");        }
	}

	//Test operator+ (r-value, l-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2, 2, {10.2, 20.4, 20.6, 7.2}};
		Matrix<double> res = std::move(a) + b;
		if(a.data.size() != 0)                              { err("operator+ test (r-value, l-value) [src size]");     }
		if(b.data.size() != 4)                              { err("operator+ test (r-value, l-value) [src size]");     }
		if(res.data.size() != 4)                            { err("operator+ test (r-value, l-value) [size]");         }
		if(b(0, 0) != 7.1 || b(0, 1) != 15.2 || b(1, 0) != 11.3 || b(1, 1) != 3.1){ err("operator+ test (r-value, l-value) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )                  { err("operator+ test (r-value, l-value) [value]");        }
	}

	//Test operator+ (l-value, r-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2, 2, {10.2, 20.4, 20.6, 7.2}};
		Matrix<double> res = a + std::move(b);
		if(a.data.size() != 4)                              { err("operator+ test (l-value, r-value) [src size]");     }
		if(b.data.size() != 0)                              { err("operator+ test (l-value, r-value) [src size]");     }
		if(res.data.size() != 4)                            { err("operator+ test (l-value, r-value) [size]");         }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1){ err("operator+ test (l-value, r-value) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )                  { err("operator+ test (l-value, r-value) [value]");        }
	}

	//Test operator+ (r-value, r-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2, 2, {10.2, 20.4, 20.6, 7.2}};
		Matrix<double> res = std::move(a) + std::move(b);
		if(a.data.size() != 0)            { err("operator+ test (r-value, r-value) [src size]"); }
		if(b.data.size() != 4)            { err("operator+ test (r-value, r-value) [src size]"); }//this argument was not reused!
		if(res.data.size() != 4)          { err("operator+ test (r-value, r-value) [size]");     }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   ){ err("operator+ test (r-value, r-value) [value]");    }
	}

	//Test +=:
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2, 2, {10.2, 20.4, 20.6, 7.2}};
	    a += b;
		if(a.data.size() != 4)                              { err("+= test [size]");         }
		if(b.data.size() != 4)                              { err("+= test [src size]");     }
		if(b(0, 0) != 7.1 || b(0, 1) != 15.2 || b(1, 0) != 11.3 || b(1, 1) != 3.1){ err("+= test [src elements]"); }
		if(std::abs(ref(0, 0)-a(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-a(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-a(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-a(1, 1)) > 1e-15   )                    { err("+= test [value]");        }
		
	}

	//Test -=:
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2,2,{4.0, 10.0, 2.0, -1.0}};
		b -= a;
		if(a.data.size() != 4)                            { err("-= test [size]");         }
		if(b.data.size() != 4)                            { err("-= test [src size]");     }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1){ err("-= test [src elements]"); }
		if(std::abs(ref(0, 0)-b(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-b(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-b(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-b(1, 1)) > 1e-15   )                  { err("-= test [value]");        }
	}

	//Test *=:
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> ref = {2,2,{6.2, 10.4, 18.6, 8.2}};
		a *= 2.0;
		if(a.data.size() != 4)          { err("*= test [size]");  }
		if(std::abs(ref(0, 0)-a(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-a(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-a(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-a(1, 1)) > 1e-15   ) { err("*= test [value]"); }
	}

	//Test operator* (l-value, scalar)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> ref = {2,2,{6.2, 10.4, 18.6, 8.2}};
		Matrix<double> res = a * 2.0;
		if(a.data.size()   != 4)                           { err("operator* test (l-value, scalar) [src size]");     }
		if(res.data.size() != 4)                           { err("operator* test (l-value, scalar) [size]");         }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1){ err("operator* test (l-value, scalar) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )                 { err("operator* test (l-value, scalar) [value]");        }
	}

	//Test operator* (r-value, scalar)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> ref = {2,2,{6.2, 10.4, 18.6, 8.2}};
		Matrix<double> res = std::move(a) * 2.0;
		if(a.data.size()   != 0)          { err("operator* test (r-value, scalar) [src size]");     }
		if(res.data.size() != 4)          { err("operator* test (r-value, scalar) [size]");         }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   ) { err("operator* test (r-value, scalar) [value]");        }
	}

	//Test operator* (scalar, l-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> ref = {2,2,{6.2, 10.4, 18.6, 8.2}};
		Matrix<double> res = 2.0 * a;
		if(a.data.size()   != 4)                           { err("operator* test (scalar, l-value) [src size]");     }
		if(res.data.size() != 4)                           { err("operator* test (scalar, l-value) [size]");         }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1){ err("operator* test (scalar, l-value) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )                 { err("operator* test (scalar, l-value) [value]");        }
	}

	//Test operator* (scalar, r-value)
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> ref = {2,2,{6.2, 10.4, 18.6, 8.2}};
		Matrix<double> res = 2.0 * std::move(a);
		if(a.data.size()   != 0)          { err("operator* test (scalar, r-value) [src size]");     }
		if(res.data.size() != 4)          { err("operator* test (scalar, r-value) [size]");         }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   ) { err("operator* test (scalar, r-value) [value]");        }
	}

	//Test /=:
	{
		Matrix<double> a   = {2,2,{2.6, 4.2, 8.4, 4.8}};
		Matrix<double> ref = {2,2,{1.3, 2.1, 4.2, 2.4}};
		a /= 2.0;
		if(a.data.size() != 4)          { err("/= test [size]");  }
		if(std::abs(ref(0, 0)-a(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-a(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-a(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-a(1, 1)) > 1e-15   ){ err("/= test [value]"); }
	}

	//Test operator/ (l-value, scalar)
	{
		Matrix<double> a   = {2,2,{2.6, 4.2, 8.4, 4.8}};
		Matrix<double> ref = {2,2,{1.3, 2.1, 4.2, 2.4}};
		Matrix<double> res = a / 2.0;
		if(a.data.size()   != 4)                             { err("operator/ test (l-value, scalar) [src size]");     }
		if(res.data.size() != 4)                             { err("operator/ test (l-value, scalar) [size]");         }
		if(a(0, 0) != 2.6 || a(0, 1) != 4.2 || a(1, 0) != 8.4 || a(1, 1) != 4.8){ err("operator/ test (l-value, scalar) [src elements]"); }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   )                  { err("operator/ test (l-value, scalar) [value]");        }
	}

	//Test operator/ (r-value, scalar)
	{
		Matrix<double> a   = {2,2,{2.6, 4.2, 8.4, 4.8}};
		Matrix<double> ref = {2,2,{1.3, 2.1, 4.2, 2.4}};
		Matrix<double> res = std::move(a) / 2.0;
		if(a.data.size()   != 0)          { err("operator/ test (r-value, scalar) [src size]"); }
		if(res.data.size() != 4)          { err("operator/ test (r-value, scalar) [size]");     }
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-15 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-15 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-15 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-15   ) { err("operator/ test (r-value, scalar) [value]");    }
	}

	//Test dot:
	{
		Matrix<double> a   = {2,2,{3.1, 5.2, 9.3, 4.1}};
		Matrix<double> b   = {2,2,{7.1, 15.2, 11.3, 3.1}};
		Matrix<double> ref = {2,2,{80.77, 63.24, 112.36, 154.07}};
		Matrix<double> res = dot(a, b);
		if(std::abs(ref(0, 0)-res(0, 0)) > 1e-13 ||
		   std::abs(ref(0, 1)-res(0, 1)) > 1e-13 || 
		   std::abs(ref(1, 0)-res(1, 0)) > 1e-13 ||
			 std::abs(ref(1, 1)-res(1, 1)) > 1e-13   )                         { err("dot test [value]");        }
		if(a.data.size() != 4)                             { err("dot test [src size]");     }
		if(b.data.size() != 4)                             { err("dot test [src size]");     }
		if(a(0, 0) != 3.1 || a(0, 1) != 5.2 || a(1, 0) != 9.3 || a(1, 1) != 4.1){ err("dot test [src elements]"); }
		if(b(0, 0) != 7.1 || b(0, 1) != 15.2 || b(1, 0) != 11.3 || b(1, 1) != 3.1){ err("dot test [src elements]"); }
	}

    std::cout << "All tests are successful!\n";
return 0;
}
