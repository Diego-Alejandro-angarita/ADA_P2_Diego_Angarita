
#include <iostream>
#include <string>
#include "classic_mult.hpp"
#include "karatsuba.hpp"

using namespace std;

int main(int argc, char* argv[]){
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " A B [umbral]\n";
        return 1;
    }

    try {
        Numero A = desde_cadena(argv[1]);
        Numero B = desde_cadena(argv[2]);
        size_t umbral = (argc > 3) ? stoul(argv[3]) : 1;

        Numero clasica = multiplicacionClasica(A, B);
        Numero kara = karatsuba(A, B, umbral);

        cout << "Clasica   : " << a_cadena(clasica) << '\n';
        cout << "Karatsuba : " << a_cadena(kara) << '\n';
        cout << (clasica == kara ? "Coinciden" : "NO coinciden") << '\n';
        return clasica == kara ? 0 : 2;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}