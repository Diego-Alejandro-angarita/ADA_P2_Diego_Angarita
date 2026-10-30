 #include<algorithm>
#include "karatsuba.hpp"
using namespace std;

static Numero karatsubaRec(Numero X, Numero Y, size_t umbral){
    size_t n = max(X.size(), Y.size());
    if (n <= umbral){
        return multiplicacionClasica(X, Y);
    }

    rellenar(X, n);
    rellenar(Y, n);
    size_t m = n / 2;

    auto [x1, x0] = partir(X, m);
    auto [y1, y0] = partir(Y, m);

    Numero z2 = karatsubaRec(x1, y1, umbral);                     
    Numero z0 = karatsubaRec(x0, y0, umbral);                        
    Numero z1 = karatsubaRec(sumar(x1, x0), sumar(y1, y0), umbral);  

    Numero centro = restar(restar(z1, z2), z0);

    Numero R = sumar(sumar(desplazar(z2, 2 * m), desplazar(centro, m)), z0);
    normalizar(R);
    return R;
}

Numero karatsuba(const Numero& A, const Numero& B, size_t umbral){
    if (umbral < 1) umbral = 1;
    Numero X = A, Y = B;
    normalizar(X);
    normalizar(Y);

    size_t n = potenciaDe2(max(X.size(), Y.size()));
    rellenar(X, n);
    rellenar(Y, n);

    return karatsubaRec(X, Y, umbral);
}
