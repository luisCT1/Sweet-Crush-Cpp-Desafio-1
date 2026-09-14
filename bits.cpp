#include "bits.h"

#include <cstdlib>
#include <ctime>

void iniciarAzar()
{
    //esto va una sola vez al arrancar. Si lo pongo cada vez que pido una ficha,
    //como time cambia por segundos, en el mismo segundo me salen todas iguales
    srand(time(nullptr));
}

int fichaAlAzar()
{
    //% 6 deja un numero entre 0 y 5, que son los 6 tipos
    return rand() % 6;
}

//saca los 3 bits de esa casilla y los devuelve como numero
unsigned char leerFicha(unsigned char* datos, int indice)
{
    int bitInicio = indice * 3;
    int nByte = bitInicio / 8;
    int offset = bitInicio % 8; //cuento desde el bit 0, el de la derecha

    //si desde el offset todavia caben 3 bits, la ficha esta en un solo byte
    //el 7 es 111 en binario, con eso dejo pasar solo esos 3 bits.
    if (offset + 3 <= 8) {
        return (datos[nByte] >> offset) & 7;
    }

    //si no caben, se parte. Por ejemplo si empieza en el bit 6,
    //en este byte alcanzan 2 bits y el que falta se va al siguiente
    int bitsAqui = 8 - offset;
    unsigned char mascaraAqui = (1 << bitsAqui) - 1;
    unsigned char parteBaja = (datos[nByte] >> offset) & mascaraAqui;

    int bitsAlla = 3 - bitsAqui;
    unsigned char mascaraAlla = (1 << bitsAlla) - 1;
    unsigned char parteAlta = datos[nByte + 1] & mascaraAlla;

    //junto lo de este byte con lo del otro para armar el numero de la ficha
    return parteBaja | (parteAlta << bitsAqui);
}

//guarda el valor de la ficha en esos 3 bits, sin tocar las de al lado
void escribirFicha(unsigned char* datos, int indice, unsigned char valor)
{
    //por si llega un numero mas grande, me quedo solo con 3 bits
    valor = valor & 7;

    int bitInicio = indice * 3;
    int nByte = bitInicio / 8;
    int offset = bitInicio % 8;

    if (offset + 3 <= 8) {
        //la mascara marca donde va la ficha. Con ~ apago esos bits
        //y dejo el resto del byte como estaba, despues meto el valor nuevo
        unsigned char mascara = 7 << offset;
        datos[nByte] = (datos[nByte] & ~mascara) | (valor << offset);
        return;
    }

    //misma idea que en leer, pero para guardar. Primero lo que cabe aqui
    //y lo que sobra lo escribo en el byte de al lado
    int bitsAqui = 8 - offset;
    unsigned char mascaraAqui = ((1 << bitsAqui) - 1) << offset;
    unsigned char parteBaja = valor & ((1 << bitsAqui) - 1);
    datos[nByte] = (datos[nByte] & ~mascaraAqui) | (parteBaja << offset);

    int bitsAlla = 3 - bitsAqui;
    unsigned char mascaraAlla = (1 << bitsAlla) - 1;
    unsigned char parteAlta = valor >> bitsAqui;
    datos[nByte + 1] = (datos[nByte + 1] & ~mascaraAlla) | (parteAlta & mascaraAlla);
}
