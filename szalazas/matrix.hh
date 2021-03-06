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


    Matrix():N{0}, M{0}, data{std::vector<T>()} {}
    Matrix(int nn, int mm):N{nn}, M{mm}, data(nn*mm) {}
    Matrix(int nn, int mm, std::vector<T> const & v):N{nn}, M{mm}, data{v} {}
    Matrix(int nn, int mm, std::vector<T> && v):N{nn}, M{mm}, data{std::move(v)} {}

    template<typename F>
    Matrix(int nn, int mm, F f):N{nn}, M{mm}, data(nn*mm) {
        for (int k=0; k<nn; ++k) {
            for (int l=0; l<mm; ++l) {(*this)(k, l)=f(k, l);}
        }
    }

    Matrix(Matrix<T> const & m)=default;
    Matrix(Matrix<T> && m):N{m.N}, M{m.M}, data{std::move(m.data)}{
        m.N=0;
        m.M=0;
    }

    Matrix<T>& operator=(Matrix<T> const & m)=default;
    Matrix<T>& operator=(Matrix<T> && m){
        if(&m == this){return *this;}
        N=m.N;
        M=m.M;
        data=std::move(m.data);
        m.N=0;
        m.M=0;

        return *this;
    }

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
    Matrix<T> result(m1.N, m1.M);
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
    Matrix<T> result(m1.N, m1.M);
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
    Matrix<T> result(m.N, m.M);
    std::transform(m.data.begin(), m.data.end(), result.data.begin(),
                    [&](T const & element_m){return element_m*a;});
    return result;
}
template<typename T>
Matrix<T> operator* (T const & a, Matrix<T> const & m){
    Matrix<T> result(m.N, m.M);
    std::transform(m.data.begin(), m.data.end(), result.data.begin(),
                    [&](T const & element_m){return a*element_m;});
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
    Matrix<T> result(m.N, m.M);
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
Matrix<T> operator* (Matrix<T> const & m1, Matrix<T> const & m2){
    Matrix<T> result(m1.N, m2.M);
    for (int k=0; k<m1.N; ++k){ //step on result row
        for (int l=0; l<m2.M; ++l){ //step on result column
            T accumulator=0;
            for (int j=0; j<m2.N; ++j){ //row*column
                accumulator+=m1(k, j)*m2(j, l);
            }
            result(k, l)=accumulator;
        }
    }

    return result;
}


template<typename T>
std::ostream & operator<< (std::ostream & o, Matrix<T> const & m) {
    o << m.N << '\t' << m.M << '\n';
    for (int k=0; k<m.N; ++k){
        for (int l=0; l<m.M; ++l){
            o<<m(k, l)<<'\t';
        }
        o<<'\n';
    }
    o<<'\n';

    return o;
}


/*
In the beggining there should be two integers, corresponding to the size of Matrix.
Simply reads data in Matrix element by element (doesn't check the structure).
(Tries to read as many elements as the size of Matrix.)
Everything should be separated with (any kind of) whitespaces.
*/
template<typename T>
std::istream & operator>> (std::istream & s, Matrix<T> & m) {
    const auto state = s.rdstate();
    const auto pos = s.tellg();    

    bool fail=false;

    int nn;
    int mm;
    s >> nn;
    s >> mm;

    if(s.fail()) {
        std::cout << "Error at read in the size of Matrix. (Stream is reseted to input state, Matrix didn't change.)\n";
        fail=true;
    } else if ( (nn == 0) || (mm == 0) ){
        std::cout << "Can not read in empty Matrix. (N or M is zero.) (Stream is reseted to input state, Matrix didn't change.)\n";
        fail=true;
    } 
    
    if (!fail){
        std::vector<T> temp(nn*mm);
        for (int k=0; k<nn*mm; ++k) {
            s >> temp[k];
            if (s.fail()) {
                std::cout << "Something went wrong during read data (with >>) into Matrix at element: (" << k/mm << ", " << k-k/mm << ")\n\t(Stream is reseted to input state, Matrix didn't change.)\n";
                fail=true;
                break;
            }
        }

        if (!fail) {
            m.N=nn;
            m.M=mm;
            m.data=std::move(temp);
        }
    }

    if (fail) {
        s.seekg(pos);
        s.clear();
        s.setstate(state);
    }

    return s;
    
}

//Multi threading
#include <future>

template<typename T>
Matrix<T> multiplicationMultithread (Matrix<T> const & m1, Matrix<T> const & m2, int numOfThreads){
    Matrix<T> result(m1.N, m2.M);
    std::vector<std::future<void>> futures(numOfThreads);

    auto partialMult = [&](int row_min, int row_max){
        for (int k=row_min; k<row_max; ++k){ //step on result row
            for (int l=0; l<m2.M; ++l){ //step on result column
                T accumulator=0;
                for (int j=0; j<m2.N; ++j){ //row*column
                    accumulator+=m1(k, j)*m2(j, l);
                }
                result(k, l)=accumulator;
            }
        }
    };

    for (int n=0; n<numOfThreads; ++n){
        int row_min = n*m1.N/numOfThreads;
        int row_max = (n+1)*m1.N/numOfThreads;
        futures[n] = std::async(std::launch::async, partialMult, row_min, row_max);
    }
    std::for_each( futures.begin(), futures.end(), [](std::future<void> & f){f.get();} );

    return result;
}


