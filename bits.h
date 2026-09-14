#ifndef BITS_H
#define BITS_H

//en este archivo esta lo de mover los bits y lo del azar
//el tablero no lo armo como matriz. Va en un unsigned char*
//y las fichas quedan pegadas, cada una con 3 bits

//con 3 bits salen 8 numeros. Del 0 al 5 son las fichas,
//el 6 (110) lo dejo para una casilla vacia
//y el 7 por ahora no lo uso
const unsigned char FICHA_MIN = 0;
const unsigned char FICHA_MAX = 5;
const unsigned char FICHA_VACIA = 6;
const unsigned char FICHA_NO_USADA = 7;

//arranca el azar. solo lo llamo una vez al inicio
void iniciarAzar();
//devuelve un tipo de ficha al azar, del 0 al 5
int fichaAlAzar();

//el indice es la casilla contando de izquierda a derecha y de arriba abajo
//el primer bit de esa ficha queda en indice * 3
//saca los 3 bits de esa casilla y los devuelve como numero
unsigned char leerFicha(unsigned char* datos, int indice);
//se guarda el valor de la ficha en esos 3 bits, sin tocar las de al lado
void escribirFicha(unsigned char* datos, int indice, unsigned char valor);

#endif
