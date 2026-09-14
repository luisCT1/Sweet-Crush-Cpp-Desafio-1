#include <iostream>
#include "bits.h"
#include "tablero.h"

using namespace std;

int main()
{
    int F = 0;
    int C = 0;

    //pongo flush para que se vea la pregunta antes de quedarse esperando el cin
    cout << "filas: " << flush;
    cin >> F;
    cout << "columnas: " << flush;
    cin >> C;

    if (cin.fail() || F < 1 || C < 1) {
        cout << "no se pudieron leer F y C. hay que correrlo en consola." << endl;
        return 0;
    }

    iniciarAzar();

    unsigned char* datos = crearTablero(F, C);
    int nBytes = bytesDelTablero(F, C);

    cout << endl;
    cout << "F = " << F << ", C = " << C << ", bytes = " << nBytes << endl;
    cout << endl;

    //primero la trama en binario y despues el tablero con simbolos
    //no borro lo que ya salio en la consola
    mostrarBinario(datos, nBytes);
    cout << endl;
    mostrarTablero(datos, F, C);

    liberarTablero(datos);
    return 0;
}
