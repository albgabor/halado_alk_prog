#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cmath>
#include <ostream>

template <typename T>
struct Matrix
{
    int N, M;
    std::vector<T> data;

    T & operator()(int i, int j){
        return data[M*i+j];
    }

    T const & operator()(int i, int j) const {
        return data[M*i+j];
    }


    Matrix()=default;
    Matrix(Matrix const &)=default;
    Matrix(Matrix &&)=default;

    Matrix<T>& operator=(Matrix const &)=default;
    Matrix<T>& operator=(Matrix &&)=default;

    Matrix<T>& operator+= (Matrix<T> const & m){
        std::transform(data.begin(), data.end(), m.data.begin(), data.begin(),
                       [](T const & element_this, T const & element_m){return element_this+element_m;});
        return *this;                    
    }
    Matrix<T>& operator-= (Matrix<T> const & m){
        std::transform(data.begin(), data.end(), m.data.begin(), data.begin(),
                       [](T const & element_this, T const & element_m){return element_this-element_m;});
        return *this;                    
    }

    Matrix<T>& operator*= (T const & a){
        std::transform(data.begin(), data.end(), data.begin(),
                        [&](T const & element_this){return element_this*a;});
        return *this;
    }
    Matrix<T>& operator/= (T const & a){
        std::transform(data.begin(), data.end(), data.begin(),
                        [&](T const & element_this){return element_this/a;});
        return *this;
    }


};


template<typename T>
Matrix<T> operator+ (Matrix<T> const & m1, Matrix<T> const & m2){
    Matrix<T> result={m1.N, m1.M};
    result.data.resize(result.N*result.M);
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), result.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1+element_m2;});
        return result;                    
}
template<typename T>
Matrix<T>&& operator+ (Matrix<T> && m1, Matrix<T> const & m2){
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), m1.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1+element_m2;});
        return std::move(m1);                    
}
template<typename T>
Matrix<T>&& operator+ (Matrix<T> const & m1, Matrix<T> && m2){
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), m2.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1+element_m2;});
        return std::move(m2);                    
}
template<typename T>
Matrix<T>&& operator+ (Matrix<T> && m1, Matrix<T> && m2){
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), m1.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1+element_m2;});
        return std::move(m1);                    
}

template<typename T>
Matrix<T> operator- (Matrix<T> const & m1, Matrix<T> const & m2){
    Matrix<T> result={m1.N, m1.M};
    result.data.resize(result.N*result.M);
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), result.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1-element_m2;});
        return result;                    
}
template<typename T>
Matrix<T>&& operator- (Matrix<T> && m1, Matrix<T> const & m2){
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), m1.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1-element_m2;});
        return std::move(m1);                    
}
template<typename T>
Matrix<T>&& operator- (Matrix<T> const & m1, Matrix<T> && m2){
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), m2.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1-element_m2;});
        return std::move(m2);                    
}
template<typename T>
Matrix<T>&& operator- (Matrix<T> && m1, Matrix<T> && m2){
        std::transform(m1.data.begin(), m1.data.end(), m2.data.begin(), m1.data.begin(),
                       [](T const & element_m1, T const & element_m2){return element_m1-element_m2;});
        return std::move(m1);                    
}

template<typename T>
Matrix<T> operator* (Matrix<T> const & m, T const & a){
    Matrix<T> result={m.N, m.M};
    result.data.resize(result.N*result.M);
    std::transform(m.data.begin(), m.data.end(), result.data.begin(),
                    [&](T const & element_m){return element_m*a;});
    return result;
}
template<typename T>
Matrix<T> operator* (T const & a, Matrix<T> const & m){
    Matrix<T> result={m.N, m.M};
    result.data.resize(result.N*result.M);
    std::transform(m.data.begin(), m.data.end(), result.data.begin(),
                    [&](T const & element_m){return element_m*a;});
    return result;
}
template<typename T>
Matrix<T> && operator* (Matrix<T> && m, T const & a){
    std::transform(m.data.begin(), m.data.end(), m.data.begin(),
                    [&](T const & element_m){return element_m*a;});
    return std::move(m);
}
template<typename T>
Matrix<T> && operator* ( T const & a, Matrix<T> && m){
    std::transform(m.data.begin(), m.data.end(), m.data.begin(),
                    [&](T const & element_m){return element_m*a;});
    return std::move(m);
}

template<typename T>
Matrix<T> operator/ (Matrix<T> const & m, T const & a){
    Matrix<T> result={m.N, m.M};
    result.data.resize(result.N*result.M);
    std::transform(m.data.begin(), m.data.end(), result.data.begin(),
                    [&](T const & element_m){return element_m/a;});
    return result;
}
template<typename T>
Matrix<T> && operator/ (Matrix<T> && m, T const & a){
    std::transform(m.data.begin(), m.data.end(), m.data.begin(),
                    [&](T const & element_m){return element_m/a;});
    return std::move(m);
}

template<typename T>
Matrix<T> dot (Matrix<T> const & m1, Matrix<T> const & m2){
    Matrix<T> result={m1.N, m2.M, std::vector<T>(m1.N*m2.M, 0)};
    for (int k=0; k<m1.N; ++k){ //step on result row
        for (int l=0; l<m2.M; ++l){ //step on result column
            for (int j=0; j<m2.N; ++j){ //row*column
                result.data[result.M*k+l]+=m1.data[m1.M*k+j]*m2.data[m2.M*j+l];
            }
        }
    }

    return result;
}
