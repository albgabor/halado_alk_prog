#include <cmath>
#include <iostream>


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

//The type of return value will be double in all cases!!!
template <typename T>
double length (Vector2<T> const & a){
    return std::sqrt(sqlength(a));
}

 //The type of return value will be Vector2<double> in all cases!!!
template <typename T>
Vector2<double> normalize (Vector2<T> const & a){
    
    if ((a.x == T(0)) && (a.y == T(0))) {
        std::cout << "The (0, 0) vector can't be normalized! (0.0, 0.0) is returned.\n";
        return Vector2<double>{0.0, 0.0};
    } else {
        double len=sqlength(a);
        return Vector2<double> {a.x/len, a.y/len};
    }
}

template <typename T>
std::ostream & operator<< ( std::ostream& o, Vector2<T> const & v){
    o<<v.x <<"   "<<v.y;
    return o;
}

template <typename T>
std::istream & operator>> ( std::istream& i, Vector2<T> & v){
    i>>v.x;
    i>>v.y;
    return i;
}