#include <iostream>
#include <random>
#include <string>
#include "../src/classic_mult.hpp"
#include "../src/karatsuba.hpp"

using namespace std;

static int pruebas = 0;
static int fallos = 0;

static void verificar(bool ok, const string& nombre)
{
    ++pruebas;
    if (!ok) {
        ++fallos;
        cout << "  [FALLA] " << nombre << '\n';
    }
}

static string recortar(const string& s)
{
    return s.size() <= 40 ? s : s.substr(0, 18) + "..." + s.substr(s.size() - 18);
}

static void probarProducto(const string& a, const string& b, const string& esperado = "")
{
    Numero A = desde_cadena(a), B = desde_cadena(b);
    string c   = a_cadena(multiplicacionClasica(A, B));
    string k1  = a_cadena(karatsuba(A, B, 1));
    string k32 = a_cadena(karatsuba(A, B, 32));
    string nombre = recortar(a) + " x " + recortar(b);

    verificar(k1 == c,  nombre + " (karatsuba u=1 vs clasica)");
    verificar(k32 == c, nombre + " (karatsuba u=32 vs clasica)");
    if (!esperado.empty()) verificar(c == esperado, nombre + " (clasica vs esperado)");
}

static string digitosAleatorios(mt19937& gen, size_t n, bool primeroNoCero = true)
{
    uniform_int_distribution<int> dig(0, 9), dig19(1, 9);
    string s;
    for (size_t i = 0; i < n; ++i)
        s.push_back(char('0' + ((i == 0 && primeroNoCero) ? dig19(gen) : dig(gen))));
    return s;
}

int main()
{
    // ------------------------------------------------------------ auxiliares
    cout << "Operaciones auxiliares\n";
    verificar(a_cadena(desde_cadena("000123")) == "123", "desde_cadena quita ceros");
    verificar(a_cadena(desde_cadena("0000")) == "0", "desde_cadena de solo ceros");
    verificar(a_cadena(sumar(desde_cadena("999"), desde_cadena("1"))) == "1000", "sumar 999+1 (digito extra)");
    verificar(a_cadena(sumar(desde_cadena("5"), desde_cadena("123456"))) == "123461", "sumar longitudes distintas");
    verificar(sumar(desde_cadena("99"), desde_cadena("99")).size() == 3, "sumar da a lo sumo un digito mas");
    verificar(a_cadena(restar(desde_cadena("1000"), desde_cadena("1"))) == "999", "restar con prestamos y normaliza");
    verificar(a_cadena(restar(desde_cadena("123456"), desde_cadena("123456"))) == "0", "restar iguales da 0");
    {
        Numero B = desde_cadena("7");
        rellenar(B, 6);   // B con ceros a la izquierda, mas largo que A
        verificar(a_cadena(restar(desde_cadena("50"), B)) == "43", "restar con B mas largo (ceros a la izq.)");
    }
    {
        bool lanzo = false;
        try { restar(desde_cadena("5"), desde_cadena("7")); } catch (const logic_error&) { lanzo = true; }
        verificar(lanzo, "restar detecta A < B");
    }
    verificar(a_cadena(desplazar(desde_cadena("123"), 4)) == "1230000", "desplazar 123 por 10^4");
    verificar(a_cadena(desplazar(desde_cadena("0"), 3)) == "0000", "desplazar 0 (sin normalizar)");
    {
        auto [alta, baja] = partir(desde_cadena("12345678"), 4);
        verificar(a_cadena(alta) == "1234" && a_cadena(baja) == "5678", "partir 12345678 en m=4");
        auto [alta2, baja2] = partir(desde_cadena("10000005"), 4);
        verificar(alta2.size() == 4 && baja2.size() == 4 && a_cadena(baja2) == "0005",
                  "partir conserva ceros de la mitad baja");
    }
    verificar(potenciaDe2(1) == 1 && potenciaDe2(5) == 8 && potenciaDe2(64) == 64, "potenciaDe2");

    // ------------------------------------------------------------ casos de borde
    cout << "Numeros de 1 digito\n";
    for (int a = 0; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b)
            probarProducto(to_string(a), to_string(b), to_string(a * b));
    probarProducto("0", "98765432109876543210", "0");

    cout << "Ceros a la izquierda\n";
    probarProducto("000123", "456", "56088");
    probarProducto("0000000000000000000000000000001", "0000000000009", "9");
    probarProducto("00000000000000000000000000000000000000000012345", "000678", "8369910");

    cout << "Ceros intercalados\n";
    probarProducto("100200300", "4005006", "401302802701800");
    probarProducto("1000000001", "1000000001", "1000000002000000001");
    probarProducto("10101010101010101", "909090909");

    cout << "Longitudes impares antes del relleno\n";
    probarProducto("123", "45678", "5618394");
    probarProducto("12345", "6789012", "83810353140");
    probarProducto("1234567", "7654321", "9449772114007");
    {
        mt19937 gen(127);
        probarProducto(digitosAleatorios(gen, 127), digitosAleatorios(gen, 127));
        probarProducto(digitosAleatorios(gen, 129), digitosAleatorios(gen, 63));
    }

    cout << "Oraculos independientes\n";

    for (size_t n : {1u, 2u, 3u, 7u, 16u, 31u, 100u, 257u, 600u}) {
        string nueves(n, '9');
        string esperado = string(n - 1, '9') + "8" + string(n - 1, '0') + "1";
        probarProducto(nueves, nueves, esperado);
    }

    for (size_t a : {0u, 1u, 5u, 64u, 300u})
        for (size_t b : {0u, 3u, 127u})
            probarProducto("1" + string(a, '0'), "1" + string(b, '0'), "1" + string(a + b, '0'));

    cout << "Productos grandes\n";
    {
        mt19937 gen(2026);
        probarProducto(digitosAleatorios(gen, 500), digitosAleatorios(gen, 500));
        probarProducto(digitosAleatorios(gen, 1000), digitosAleatorios(gen, 1000));
        probarProducto(digitosAleatorios(gen, 1024), digitosAleatorios(gen, 1024));
        probarProducto(string(1024, '9'), string(1024, '9'));   // maximo acarreo
    }

    cout << "Pares aleatorios\n";
    {
        mt19937 gen(42);
        uniform_int_distribution<size_t> largo(1, 300);
        for (int i = 0; i < 1000; ++i) {
            size_t la = largo(gen);
            size_t lb = (i % 3 == 0) ? largo(gen) : la;
            bool ceros = (i % 5 == 0);
            probarProducto(digitosAleatorios(gen, la, !ceros), digitosAleatorios(gen, lb, !ceros));
        }
    }

    cout << '\n' << (pruebas - fallos) << " / " << pruebas << " pruebas correctas\n";
    return fallos == 0 ? 0 : 1;
}