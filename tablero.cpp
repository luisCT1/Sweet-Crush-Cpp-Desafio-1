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

//deja la casilla en 110, o sea vacia
void borrarFicha(unsigned char* datos, int C, int fila, int col)
{
    escribirFicha(datos, fila * C + col, FICHA_VACIA);
}

//en esa columna las fichas bajan y arriba se meten fichas nuevas
void caerColumna(unsigned char* datos, int F, int C, int col)
{
    //empiezo abajo y voy subiendo. si la casilla tiene ficha la pego
    //lo mas abajo que pueda. las vacias las salto
    int destino = F - 1;
    for (int fila = F - 1; fila >= 0; fila--) {
        unsigned char ficha = leerFicha(datos, fila * C + col);
        if (ficha != FICHA_VACIA) {
            escribirFicha(datos, destino * C + col, ficha);
            destino--;
        }
    }

    //lo que quedo libre arriba lo lleno al azar
    for (int fila = 0; fila <= destino; fila++) {
        escribirFicha(datos, fila * C + col, fichaAlAzar());
    }
}

//marco las casillas que estan en un grupo de 3 o mas iguales
void marcarGrupos(unsigned char* datos, unsigned char* marcas, int F, int C)
{
    int n = F * C;
    for (int i = 0; i < n; i++) {
        marcas[i] = 0;
    }

    //primero por filas
    for (int fila = 0; fila < F; fila++) {
        int col = 0;
        while (col < C) {
            unsigned char tipo = leerFicha(datos, fila * C + col);
            if (tipo > FICHA_MAX) {
                col++;
                continue;
            }
            int ini = col;
            while (col < C && leerFicha(datos, fila * C + col) == tipo) {
                col++;
            }
            if (col - ini >= 3) {
                for (int k = ini; k < col; k++) {
                    marcas[fila * C + k] = 1;
                }
            }
        }
    }

    //despues por columnas
    for (int col = 0; col < C; col++) {
        int fila = 0;
        while (fila < F) {
            unsigned char tipo = leerFicha(datos, fila * C + col);
            if (tipo > FICHA_MAX) {
                fila++;
                continue;
            }
            int ini = fila;
            while (fila < F && leerFicha(datos, fila * C + col) == tipo) {
                fila++;
            }
            if (fila - ini >= 3) {
                for (int k = ini; k < fila; k++) {
                    marcas[k * C + col] = 1;
                }
            }
        }
    }
}

//recorre un grupo marcado. una T queda en un solo grupo porque comparten ficha
void recorrerGrupo(unsigned char* marcas, unsigned char* visto, int F, int C, int fila, int col)
{
    if (fila < 0 || fila >= F || col < 0 || col >= C) {
        return;
    }
    int i = fila * C + col;
    if (marcas[i] == 0 || visto[i] != 0) {
        return;
    }
    visto[i] = 1;
    recorrerGrupo(marcas, visto, F, C, fila - 1, col);
    recorrerGrupo(marcas, visto, F, C, fila + 1, col);
    recorrerGrupo(marcas, visto, F, C, fila, col - 1);
    recorrerGrupo(marcas, visto, F, C, fila, col + 1);
}

//cuantas combinaciones hay. si dos tiras no comparten ficha, son dos
int contarCombinaciones(unsigned char* marcas, int F, int C)
{
    int n = F * C;
    unsigned char* visto = new unsigned char[n];
    for (int i = 0; i < n; i++) {
        visto[i] = 0;
    }

    int cuantas = 0;
    for (int fila = 0; fila < F; fila++) {
        for (int col = 0; col < C; col++) {
            int i = fila * C + col;
            if (marcas[i] != 0 && visto[i] == 0) {
                cuantas++;
                recorrerGrupo(marcas, visto, F, C, fila, col);
            }
        }
    }

    delete[] visto;
    return cuantas;
}

//pone 110 en todas las marcadas y dice cuantas fichas quite
int borrarMarcadas(unsigned char* datos, unsigned char* marcas, int F, int C)
{
    int n = F * C;
    int quitadas = 0;
    for (int i = 0; i < n; i++) {
        if (marcas[i] != 0) {
            escribirFicha(datos, i, FICHA_VACIA);
            quitadas++;
        }
    }
    return quitadas;
}

//cae todo el tablero, columna por columna
void caerTablero(unsigned char* datos, int F, int C)
{
    for (int col = 0; col < C; col++) {
        caerColumna(datos, F, C, col);
    }
}

//busca 3 o mas iguales, las borra, cae y repite. no imprime en el medio
void resolverCombinaciones(unsigned char* datos, int F, int C, int* nCombos, int* nCascadas, int* nFichas)
{
    *nCombos = 0;
    *nCascadas = 0;
    *nFichas = 0;

    int n = F * C;
    while (true) {
        unsigned char* marcas = new unsigned char[n];
        marcarGrupos(datos, marcas, F, C);

        int hay = 0;
        for (int i = 0; i < n; i++) {
            if (marcas[i] != 0) {
                hay = 1;
                break;
            }
        }

        if (hay == 0) {
            delete[] marcas;
            break;
        }

        //marco todas, borro todas, despues caigo. una T cuenta como una
        *nCombos = *nCombos + contarCombinaciones(marcas, F, C);
        *nFichas = *nFichas + borrarMarcadas(datos, marcas, F, C);
        delete[] marcas;

        caerTablero(datos, F, C);
        //esta caida si es cascada, porque viene de una combinacion
        *nCascadas = *nCascadas + 1;
    }
}
