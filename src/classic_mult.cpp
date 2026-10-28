#include <stdexcept>
#include "classic_mult.hpp"

using namespace std;

void normalizar(Numero& A){
    while (A.size() > 1 && A.back() == 0){
        A.pop_back();
    } 
    if (A.empty()) A.push_back(0);
}

Numero desde_cadena(string& cadena){
    if(cadena.empty()){
        throw invalid_argument("La cadena no puede estar vacia");
    }

    Numero A;
    A.reserve(cadena.size());

    for(auto i = cadena.rbegin(); i != cadena.rend(); ++i){
        if(!isdigit(*i)){
            throw invalid_argument("La cadena contiene caracteres no numericos");
        }
        A.push_back(*i - '0');

    }

    normalizar(A);
    return A;
}

string a_cadena(const Numero& A){
    string cadena;
    cadena.reserve(A.size());

    for(auto i = A.rbegin(); i != A.rend(); ++i){
        cadena.push_back(*i + '0');
    }

    return cadena;
}

Numero multiplicacionClasica(Numero& A, Numero& B)
{
    Numero R(A.size() + B.size(), 0);
    for (size_t i = 0; i < A.size(); ++i) {
        int acarreo = 0;
        for (size_t j = 0; j < B.size(); ++j) {
            int d = R[i + j] + A[i] * B[j] + acarreo;
            R[i + j] = d % 10;
            acarreo = d / 10;
        }
        R[i + B.size()] += acarreo;   // queda < 10: es la primera vez que se escribe esa casilla
    }
    normalizar(R);
    return R;
}