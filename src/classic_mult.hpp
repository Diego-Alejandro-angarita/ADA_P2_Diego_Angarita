#ifndef CLASSIC_MULT_HPP
#define CLASSIC_MULT_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

using Numero = std::vector<int>; 

Numero desde_cadena(string& cadena);
string a_cadena(Numero& A);

Numero normalizar(Numero& A);
Numero multiplicacionClasica(Numero& A, Numero& B);

#endif