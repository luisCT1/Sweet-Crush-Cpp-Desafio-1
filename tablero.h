#ifndef TABLERO_H
#define TABLERO_H

//F son las filas y C las columnas
//datos es el arreglo de bytes donde van pegadas las fichas

//cuantos bytes hay que pedir para F filas y C columnas
int bytesDelTablero(int F, int C);
//se pide la memoria, la deja en ceros y llena cada casilla al azar
unsigned char* crearTablero(int F, int C);
//se suelta el bloque que pedi con new[]
void liberarTablero(unsigned char* datos);
//imprimo cada byte en bits, del 7 al 0
void mostrarBinario(unsigned char* datos, int nBytes);
//imprimo el tablero con simbolos y con filas y columnas numeradas
void mostrarTablero(unsigned char* datos, int F, int C);
//deja la casilla en 110, o sea vacia
void borrarFicha(unsigned char* datos, int C, int fila, int col);
//en esa columna las fichas bajan y arriba se meten fichas nuevas
void caerColumna(unsigned char* datos, int F, int C, int col);
//busca 3 o mas iguales, las borra, cae y repite. no imprime en el medio
void resolverCombinaciones(unsigned char* datos, int F, int C, int* nCombos, int* nCascadas, int* nFichas);
//insertar siempre pide un bloque nuevo. pos puede ser F para ponerla al final
unsigned char* insertarFila(unsigned char* datos, int* F, int C, int pos, int* nBytes, int* celdasReserva);
//igual para columna. pos puede ser C para ponerla al final
unsigned char* insertarColumna(unsigned char* datos, int F, int* C, int pos, int* nBytes, int* celdasReserva);
//quitar compacta en el mismo arreglo. solo si baja del 65% se hace new mas chico
unsigned char* eliminarFila(unsigned char* datos, int* F, int C, int pos, int* nBytes, int* celdasReserva);
unsigned char* eliminarColumna(unsigned char* datos, int F, int* C, int pos, int* nBytes, int* celdasReserva);

#endif
