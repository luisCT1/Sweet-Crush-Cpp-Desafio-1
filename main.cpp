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
    int celdasReserva = F * C;
    int puntajeTotal = 0;

    cout << endl;
    cout << "F = " << F << ", C = " << C << ", bytes = " << nBytes << endl;
    cout << endl;
    mostrarBinario(datos, nBytes);
    cout << endl;
    mostrarTablero(datos, F, C);

    while (true) {
        int op = 0;
        cout << endl;
        cout << "1. borrar ficha" << endl;
        cout << "2. agregar fila" << endl;
        cout << "3. quitar fila" << endl;
        cout << "4. agregar columna" << endl;
        cout << "5. quitar columna" << endl;
        cout << "6. salir" << endl;
        cout << "opcion: " << flush;
        cin >> op;

        if (cin.fail()) {
            cout << "hay que correrlo en consola." << endl;
            break;
        }
        if (op == 6) {
            break;
        }

        int combos = 0;
        int cascadas = 0;
        int fichasCombo = 0;
        int fichasJugada = 0;
        int elimUsuario = 0;

        if (op == 1) {
            int fila = -1;
            int col = -1;
            cout << "fila a borrar: " << flush;
            cin >> fila;
            cout << "columna a borrar: " << flush;
            cin >> col;
            if (fila < 0 || fila >= F || col < 0 || col >= C) {
                cout << "esa coordenada no esta en el tablero" << endl;
                continue;
            }
            //esto todavia no es cascada. solo borro una ficha y cae esa columna
            borrarFicha(datos, C, fila, col);
            caerColumna(datos, F, C, col);
            elimUsuario = 1;
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = 1 + fichasCombo;
        } else if (op == 2) {
            int pos = -1;
            cout << "posicion de la fila (0 a " << F << "): " << flush;
            cin >> pos;
            if (pos < 0 || pos > F) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            datos = insertarFila(datos, &F, C, pos, &nBytes, &celdasReserva);
            //el insertar no cuenta como cascada, pero si se buscan combinaciones
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
        } else if (op == 3) {
            if (F < 1) {
                cout << "no hay filas para quitar" << endl;
                continue;
            }
            int pos = -1;
            cout << "fila a quitar (0 a " << (F - 1) << "): " << flush;
            cin >> pos;
            if (pos < 0 || pos >= F) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            //quitar fila no es cascada, solo se cierra el hueco
            datos = eliminarFila(datos, &F, C, pos, &nBytes, &celdasReserva);
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
        } else if (op == 4) {
            int pos = -1;
            cout << "posicion de la columna (0 a " << C << "): " << flush;
            cin >> pos;
            if (pos < 0 || pos > C) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            datos = insertarColumna(datos, F, &C, pos, &nBytes, &celdasReserva);
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
        } else if (op == 5) {
            if (C < 1) {
                cout << "no hay columnas para quitar" << endl;
                continue;
            }
            int pos = -1;
            cout << "columna a quitar (0 a " << (C - 1) << "): " << flush;
            cin >> pos;
            if (pos < 0 || pos >= C) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            //quitar columna no es cascada. las fichas no caen, solo se compacta
            datos = eliminarColumna(datos, F, &C, pos, &nBytes, &celdasReserva);
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
        } else {
            cout << "opcion no valida" << endl;
            continue;
        }

        puntajeTotal = puntajeTotal + combos;

        cout << endl;
        cout << "F = " << F << ", C = " << C << ", bytes = " << nBytes << endl;
        if (celdasReserva > 0) {
            cout << "celdas ahora: " << (F * C) << ", ultima reserva: " << celdasReserva;
            cout << ", ocupacion: " << (F * C * 100 / celdasReserva) << "%" << endl;
        }
        cout << "eliminaciones del usuario: " << elimUsuario << endl;
        cout << "combinaciones: " << combos << endl;
        cout << "cascadas: " << cascadas << endl;
        cout << "fichas eliminadas: " << fichasJugada << endl;
        cout << "puntaje de la jugada: " << combos << endl;
        cout << "puntaje total: " << puntajeTotal << endl;
        cout << endl;
        mostrarBinario(datos, nBytes);
        cout << endl;
        mostrarTablero(datos, F, C);
    }

    liberarTablero(datos);
    return 0;
}
