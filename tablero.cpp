#include "tablero.h"
#include "bits.h"

#include <iostream>

using namespace std;

//calcula cuantos bytes hay que pedir para F filas y C columnas
int bytesDelTablero(int F, int C)
{
    //el +7 es porque la division es entera y necesito redondear para arriba
    //si 3*F*C no cae justo en 8, igual tengo que pedir el ultimo byte
    return (3 * F * C + 7) / 8;
}

//pide la memoria, la deja en ceros y llena cada casilla al azar
unsigned char* crearTablero(int F, int C)
{
    int nBytes = bytesDelTablero(F, C);
    unsigned char* datos = new unsigned char[nBytes];

    //lo dejo en ceros para que no quede basura. Si sobran bits,
    //se agrupan a la izquierda del ultimo byte, no en el medio de la trama
    for (int i = 0; i < nBytes; i++) {
        datos[i] = 0;
    }

    int nCasillas = F * C;
    for (int i = 0; i < nCasillas; i++) {
        escribirFicha(datos, i, fichaAlAzar());
    }

    return datos;
}

void liberarTablero(unsigned char* datos)
{
    //pedi la memoria con new[] entonces la suelto con delete[]
    delete[] datos;
}

//imprime cada byte en bits, del 7 al 0
void mostrarBinario(unsigned char* datos, int nBytes)
{
    cout << "trama (bit 7 a bit 0 de cada byte):" << endl;
    for (int i = 0; i < nBytes; i++) {
        cout << "byte " << i << ": ";
        //los imprimo de izquierda a derecha, empezando por el bit 7
        for (int b = 7; b >= 0; b--) {
            int bit = (datos[i] >> b) & 1;
            cout << bit;
            if (b > 0) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

//imprime el tablero con simbolos y con filas y columnas numeradas
void mostrarTablero(unsigned char* datos, int F, int C)
{
    //como no puedo usar string, armo un char para cada tipo de ficha
    char simbolo[6];
    simbolo[0] = '#';
    simbolo[1] = '@';
    simbolo[2] = 'O';
    simbolo[3] = 'X';
    simbolo[4] = '+';
    simbolo[5] = '%';

    cout << "tablero:" << endl;

    //arriba pongo el numero de cada columna para ubicarme
    cout << "   ";
    for (int col = 0; col < C; col++) {
        cout << col << " ";
    }
    cout << endl;

    for (int fila = 0; fila < F; fila++) {
        cout << fila << "  ";
        for (int col = 0; col < C; col++) {
            //recorro por filas, por eso el indice es fila * C + col
            int indice = fila * C + col;
            unsigned char ficha = leerFicha(datos, indice);
            if (ficha <= FICHA_MAX) {
                cout << simbolo[ficha] << " ";
            } else {
                //si esta vacia (110) pongo un punto
                cout << ". ";
            }
        }
        cout << endl;
    }
}
