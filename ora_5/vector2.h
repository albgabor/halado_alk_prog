#include <cmath>
#include <iostream>
#include <string>


template <typename T>
struct Vector2
{
    T x, y;

    Vector2<T> & operator+= (Vector2<T> const & v){
        x += T(v.x);
        y+= T(v.y);
        return *this;
    }

    Vector2<T> & operator-= (Vector2<T> const & v){
        x-=T(v.x);
        y-=T(v.y);
        return *this;
    }

    Vector2<T> & operator*= (T const & s){
        x*=T(s);
        y*=T(s);
        return *this;
    }

    Vector2<T> & operator/= (T const & s){
        x/=T(s);
        y/=T(s);
        return *this;
    }
};

template <typename T>
Vector2<T> operator+ ( Vector2<T> const & a, Vector2<T> const & b){
    return Vector2<T> {a.x+b.x, a.y+b.y};
}

template <typename T>
Vector2<T> operator- ( Vector2<T> const & a, Vector2<T> const & b){
    return Vector2<T> {a.x-b.x, a.y-b.y};
}

template <typename T>
Vector2<T> operator* ( Vector2<T> const & a, T const & b){
    return Vector2<T> {a.x*b, a.y*b};
}

template <typename T>
Vector2<T> operator* ( T const & a, Vector2<T> const & b){
    return Vector2<T> {a*b.x, a*b.y};
}

template <typename T>
Vector2<T> operator/ ( Vector2<T> const & a, T const & b){
    return Vector2<T> {a.x/b, a.y/b};
}

template <typename T>
T dot (Vector2<T> const & a, Vector2<T> const & b){
    return (a.x*b.x+a.y*b.y);
}

template <typename T>
T sqlength (Vector2<T> const & a){
    return (dot<T>(a, a));
}

//For return value see std::sqrt()
template <typename T>
auto length (Vector2<T> const & a){
    return std::sqrt(sqlength(a));
}

template <typename T>
Vector2<T> normalize (Vector2<T> const & a, double num_zero=1e-14){
    
    if (length(a)<=num_zero) {
        std::cout << "The vector is numerically zero, shouldn't be normalized! The input Vector2 is returned.\n";
        return a;
    } else {
        auto len=length(a);
        return Vector2<T> {T(a.x/len), T(a.y/len)};
    }
}

template <typename T>
std::ostream & operator<< ( std::ostream& o, Vector2<T> const & v){
    o<<v.x <<"   "<<v.y;
    return o;
}


//The >> waits for numbers, separated with whitespaces.
template <typename T>
std::istream & operator>> ( std::istream& s, Vector2<T> & v){
    const auto state = s.rdstate();
    const auto pos = s.tellg();
    
    s >> v.x;
    s >> v.y;
    if (s.fail()) {
        std::cout << "Something went wrong during read data (with >>) into Vector2. (Stream is reseted to input state.)\n";
        s.seekg(pos);
        s.setstate(state);
    }

    return s;
}