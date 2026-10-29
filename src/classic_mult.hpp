#ifndef CLASSIC_MULT_HPP
#define CLASSIC_MULT_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

using Numero = std::vector<int>; 

Numero desde_cadena(string& cadena);
string a_cadena(Numero& A);

void normalizar(Numero& A);
Numero multiplicacionClasica(Numero& A, Numero& B);

void rellenar(Numero& A, std::size_t n);

size_t potenciaDe2(size_t n);

Numero sumar(const Numero& A, const Numero& B);
Numero restar(const Numero& A, const Numero& B);
Numero desplazar(const Numero& A, std::size_t k);   
pair<Numero, Numero> partir(const Numero& A, size_t m);

#endif