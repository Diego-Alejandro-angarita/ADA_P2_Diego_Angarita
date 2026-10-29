#include <stdexcept>
#include "classic_mult.hpp"

using namespace std;

void normalizar(Numero& A){
    while (A.size() > 1 && A.back() == 0){
        A.pop_back();
    } 
    if (A.empty()) A.push_back(0);
}

void rellenar(Numero& A, size_t n)
{
    if (A.size() < n) A.resize(n, 0);   // ceros en las posiciones altas
}

size_t potenciaDe2(size_t n)
{
    size_t p = 1;
    while (p < n) p <<= 1;
    return p;
}

Numero sumar(const Numero& A, const Numero& B)
{
    const Numero& mayor = (A.size() >= B.size()) ? A : B;
    const Numero& menor = (A.size() >= B.size()) ? B : A;

    Numero R;
    R.reserve(mayor.size() + 1);
    int acarreo = 0;
    for (size_t i = 0; i < mayor.size(); ++i) {
        int d = mayor[i] + (i < menor.size() ? menor[i] : 0) + acarreo;
        R.push_back(d % 10);
        acarreo = d / 10;
    }
    // el unico digito extra posible es el acarreo final
    if (acarreo) R.push_back(acarreo);
    return R;
}

Numero restar(const Numero& A, const Numero& B)
{
    // B puede traer ceros a la izquierda y ser "mas largo" que A
    for (size_t i = A.size(); i < B.size(); ++i)
        if (B[i] != 0) throw logic_error("restar: se viola la precondicion A >= B");

    Numero R(A.size());
    int prestamo = 0;
    for (size_t i = 0; i < A.size(); ++i) {
        int d = A[i] - (i < B.size() ? B[i] : 0) - prestamo;
        if (d < 0) { d += 10; prestamo = 1; }
        else       { prestamo = 0; }
        R[i] = d;
    }
    if (prestamo) throw logic_error("restar: se viola la precondicion A >= B");
    normalizar(R);
    return R;
}

Numero desplazar(const Numero& A, size_t k)
{
    // multiplicar por 10^k = k ceros en las posiciones bajas
    Numero R(k, 0);
    R.insert(R.end(), A.begin(), A.end());
    return R;
}

pair<Numero, Numero> partir(const Numero& A, size_t m)
{
    if (m >= A.size()) return { Numero{0}, A };
    Numero baja(A.begin(), A.begin() + m);
    Numero alta(A.begin() + m, A.end());
    return { alta, baja };
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