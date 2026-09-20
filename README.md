# Sweet-Crush-Cpp-Desafio-1

Informática II · Universidad de Antioquia · 2026-2  
Luis Miguel Cohen Teherán

Programa de consola en C++ (Qt). El tablero no es una matriz: las fichas van pegadas en un `unsigned char*` pedido con `new[]`, a 3 bits por casilla.

## Cómo correrlo

1. Abrir `SweetCrush.pro` en Qt Creator.
2. Elegir el kit de escritorio.
3. En Projects → Run, marcar **Run in terminal**. Si no, `cin` no lee y el programa se cierra.
4. Construir y ejecutar.

Al arrancar pide filas y columnas (enteros ≥ 1). Filas y columnas se numeran desde 0.

## Archivos

| Archivo | Qué hace |
| --- | --- |
| `bits.h` / `bits.cpp` | Azar, leer y escribir 3 bits (también si la ficha se parte entre dos bytes). |
| `tablero.h` / `tablero.cpp` | Crear el tablero, las dos vistas, borrar, caer, combinaciones, insertar y quitar. |
| `main.cpp` | Menú y una jugada atómica por turno. |
| `SweetCrush.pro` | Proyecto Qt de consola, sin GUI. |

No se usan `struct`, `class`, `template`, `string` ni STL. La memoria va con `new[]` / `delete[]`.

## Empaquetado

Índice de la casilla: `i = fila * C + col`.  
El primer bit de esa ficha: `i * 3`.

Con 3 bits hay 8 valores:

- `0`–`5`: las seis fichas (`# @ O X + %`)
- `6` (`110`): casilla vacía
- `7` (`111`): no se usa

La reserva inicial es justa: `(3 * F * C + 7) / 8` bytes. Los bits que sobran quedan a la izquierda del último byte.

## Menú

Cada opción hace una sola cosa:

1. Borrar una ficha (cae esa columna).
2. Insertar una fila.
3. Quitar una fila.
4. Insertar una columna.
5. Quitar una columna.
6. Salir.

Si no hay filas o columnas, no se ofrece borrar ficha ni quitar.

Después de cada jugada se muestran la estadística, el tablero en bits y el tablero con símbolos.

## Combinaciones y puntaje

Se buscan tiras de 3 o más iguales, en horizontal y en vertical, al mismo tiempo. Se marcan todas, se borran, cae el tablero y se repite. Cada repetición cuenta como cascada.

El puntaje es 1 por combinación. Una T o una L cuentan como una, porque el cruce comparte ficha. La ficha que borra el usuario no suma punto.

Insertar o quitar fila/columna no cuenta como cascada. Después sí se buscan combinaciones.

## Memoria

- Insertar fila o columna: siempre `new[]` y se copia ficha a ficha (no se copia el bloque de bytes).
- Quitar: se compacta en el mismo arreglo. Solo si la ocupación baja del 65% se pide un bloque más chico.

La ocupación se compara con enteros: `usadas * 100 < reserva * 65`.
