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

    int fila = -1;
    int col = -1;
    cout << endl;
    cout << "fila a borrar: " << flush;
    cin >> fila;
    cout << "columna a borrar: " << flush;
    cin >> col;

    if (cin.fail() || fila < 0 || fila >= F || col < 0 || col >= C) {
        cout << "esa coordenada no esta en el tablero" << endl;
        liberarTablero(datos);
        return 0;
    }

    //esto todavia no es cascada. solo borro una ficha y cae esa columna
    borrarFicha(datos, C, fila, col);
    caerColumna(datos, F, C, col);

    int combos = 0;
    int cascadas = 0;
    int fichasCombo = 0;
    resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);

    cout << endl;
    cout << "combinaciones: " << combos << endl;
    cout << "cascadas: " << cascadas << endl;
    cout << "fichas eliminadas: " << (1 + fichasCombo) << endl;
    cout << "puntaje: " << combos << endl;
    cout << endl;

    mostrarBinario(datos, nBytes);
    cout << endl;
    mostrarTablero(datos, F, C);

    liberarTablero(datos);
    return 0;
}
