#include <iostream>
#include "bits.h"
#include "tablero.h"

using namespace std;

//un titulo para separar bloques en consola. uso const char* porque string no se puede
void imprimirTitulo(const char* texto)
{
    cout << endl;
    cout << "---- " << texto << " ----" << endl;
}

void lineaGruesa()
{
    cout << "================================" << endl;
}

//op 1 usa a=fila y b=columna. las demas usan solo a=posicion
void mostrarAccion(int op, int a, int b)
{
    if (op == 1) {
        cout << "se borro la ficha de la fila " << a << ", columna " << b;
    } else if (op == 2) {
        cout << "se inserto una fila en la posicion " << a;
    } else if (op == 3) {
        cout << "se quito la fila " << a;
    } else if (op == 4) {
        cout << "se inserto una columna en la posicion " << a;
    } else if (op == 5) {
        cout << "se quito la columna " << a;
    }
}

//si esJugada vale 1, tambien salen combinaciones, cascadas y puntaje
void mostrarEstadistica(int F, int C, int nBytes, int celdasReserva,
                        int elimUsuario, int combos, int cascadas,
                        int fichasJugada, int puntajeTotal, int esJugada)
{
    imprimirTitulo("Estadistica");
    cout << "filas: " << F << endl;
    cout << "columnas: " << C << endl;
    cout << "bytes del bloque: " << nBytes << endl;
    if (celdasReserva > 0) {
        cout << "celdas actuales: " << (F * C) << endl;
        cout << "celdas de la ultima reserva: " << celdasReserva << endl;
        cout << "ocupacion: " << (F * C * 100 / celdasReserva) << "%" << endl;
    }
    if (esJugada == 1) {
        cout << "eliminaciones del usuario: " << elimUsuario << endl;
        cout << "combinaciones de esta jugada: " << combos << endl;
        cout << "cascadas de esta jugada: " << cascadas << endl;
        cout << "fichas eliminadas en esta jugada: " << fichasJugada << endl;
        cout << "puntaje de esta jugada: " << combos << endl;
        cout << "puntaje total: " << puntajeTotal << endl;
    }
}

int main()
{
    int F = 0;
    int C = 0;

    cout << "================================" << endl;
    cout << "          SWEET CRUSH" << endl;
    cout << "================================" << endl;
    cout << endl;
    cout << "Para armar el tablero hay que indicar el tamano." << endl;
    cout << "Las fichas se guardan a 3 bits por casilla." << endl;
    cout << endl;

    //pongo flush para que se vea la pregunta antes de quedarse esperando el cin
    cout << "Cuantas filas quiere? Escriba un entero mayor o igual que 1: " << flush;
    cin >> F;
    cout << "Cuantas columnas quiere? Escriba un entero mayor o igual que 1: " << flush;
    cin >> C;

    if (cin.fail() || F < 1 || C < 1) {
        cout << "no se pudieron leer las filas y las columnas. hay que correrlo en consola." << endl;
        return 0;
    }

    iniciarAzar();

    unsigned char* datos = crearTablero(F, C);
    int nBytes = bytesDelTablero(F, C);
    //celdasReserva es con cuantas celdas pedi el bloque la ultima vez
    int celdasReserva = F * C;
    //el puntaje es 1 por cada combinacion y se va acumulando
    int puntajeTotal = 0;
    int nJugada = 0;
    int ultimaOp = 0;
    int ultimoA = -1;
    int ultimoB = -1;

    cout << endl;
    lineaGruesa();
    cout << "        TABLERO INICIAL" << endl;
    lineaGruesa();
    mostrarEstadistica(F, C, nBytes, celdasReserva, 0, 0, 0, 0, 0, 0);
    mostrarBinario(datos, nBytes);
    mostrarTablero(datos, F, C);
    cout << endl;
    cout << "======== fin del tablero inicial ========" << endl;

    while (true) {
        int op = 0;
        imprimirTitulo("Menu");
        if (nJugada > 0) {
            cout << "Jugada anterior (" << nJugada << "): ";
            mostrarAccion(ultimaOp, ultimoA, ultimoB);
            cout << endl;
        }
        //si no hay casillas no ofrezco borrar ficha. si no hay filas, tampoco quitar fila
        if (F >= 1 && C >= 1) {
            cout << "1. borrar una ficha (cae esa columna)" << endl;
        }
        cout << "2. agregar una fila" << endl;
        if (F >= 1) {
            cout << "3. quitar una fila" << endl;
        }
        cout << "4. agregar una columna" << endl;
        if (C >= 1) {
            cout << "5. quitar una columna" << endl;
        }
        cout << "6. salir" << endl;
        cout << "Escriba el numero de la opcion: " << flush;
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
        int accionA = -1;
        int accionB = -1;

        if (op == 1) {
            if (F < 1 || C < 1) {
                cout << "no hay fichas para borrar" << endl;
                continue;
            }
            int fila = -1;
            int col = -1;
            cout << "Indique la fila de la ficha que quiere borrar (de 0 a " << (F - 1) << "): " << flush;
            cin >> fila;
            cout << "Indique la columna de esa ficha (de 0 a " << (C - 1) << "): " << flush;
            cin >> col;
            if (cin.fail()) {
                cout << "hay que correrlo en consola." << endl;
                break;
            }
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
            accionA = fila;
            accionB = col;
        } else if (op == 2) {
            int pos = -1;
            cout << "Indique en que posicion insertar la fila." << endl;
            cout << "0 queda arriba y " << F << " queda abajo: " << flush;
            cin >> pos;
            if (cin.fail()) {
                cout << "hay que correrlo en consola." << endl;
                break;
            }
            if (pos < 0 || pos > F) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            datos = insertarFila(datos, &F, C, pos, &nBytes, &celdasReserva);
            //el insertar no cuenta como cascada, pero si se buscan combinaciones
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
            accionA = pos;
        } else if (op == 3) {
            if (F < 1) {
                cout << "no hay filas para quitar" << endl;
                continue;
            }
            int pos = -1;
            cout << "Indique cual fila quiere quitar (de 0 a " << (F - 1) << "): " << flush;
            cin >> pos;
            if (cin.fail()) {
                cout << "hay que correrlo en consola." << endl;
                break;
            }
            if (pos < 0 || pos >= F) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            //quitar fila no es cascada, solo se cierra el hueco
            datos = eliminarFila(datos, &F, C, pos, &nBytes, &celdasReserva);
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
            accionA = pos;
        } else if (op == 4) {
            int pos = -1;
            cout << "Indique en que posicion insertar la columna." << endl;
            cout << "0 queda a la izquierda y " << C << " queda a la derecha: " << flush;
            cin >> pos;
            if (cin.fail()) {
                cout << "hay que correrlo en consola." << endl;
                break;
            }
            if (pos < 0 || pos > C) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            datos = insertarColumna(datos, F, &C, pos, &nBytes, &celdasReserva);
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
            accionA = pos;
        } else if (op == 5) {
            if (C < 1) {
                cout << "no hay columnas para quitar" << endl;
                continue;
            }
            int pos = -1;
            cout << "Indique cual columna quiere quitar (de 0 a " << (C - 1) << "): " << flush;
            cin >> pos;
            if (cin.fail()) {
                cout << "hay que correrlo en consola." << endl;
                break;
            }
            if (pos < 0 || pos >= C) {
                cout << "esa posicion no sirve" << endl;
                continue;
            }
            //quitar columna no es cascada. las fichas no caen, solo se compacta
            datos = eliminarColumna(datos, F, &C, pos, &nBytes, &celdasReserva);
            resolverCombinaciones(datos, F, C, &combos, &cascadas, &fichasCombo);
            fichasJugada = fichasCombo;
            accionA = pos;
        } else {
            cout << "esa opcion no es valida" << endl;
            continue;
        }

        puntajeTotal = puntajeTotal + combos;
        nJugada = nJugada + 1;
        ultimaOp = op;
        ultimoA = accionA;
        ultimoB = accionB;

        cout << endl;
        lineaGruesa();
        cout << "           JUGADA " << nJugada << endl;
        lineaGruesa();
        cout << "Que se hizo: ";
        mostrarAccion(op, accionA, accionB);
        cout << endl;

        mostrarEstadistica(F, C, nBytes, celdasReserva,
                           elimUsuario, combos, cascadas, fichasJugada, puntajeTotal, 1);
        mostrarBinario(datos, nBytes);
        mostrarTablero(datos, F, C);
        cout << endl;
        cout << "======== fin de la jugada " << nJugada << " ========" << endl;
    }

    liberarTablero(datos);
    return 0;
}
