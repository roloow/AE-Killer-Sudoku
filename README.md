# ProyectoIA

## Compilar & Correr

Solo se requiere correr por consola el comando `make` para compilar y eso correrá el algoritmo el cual mostrará por consola tanto el tamaño de población, como la generación que se está mostrando y el fitness del mejor individuo. Abajo de estos atributos se muestra un tablero de Sudoku que representa la mejor solución de la generación.

## Constantes

Para cambiar alguna variable global basta abrir el archivo `main.cpp` y se verá en la parte inicial del código las variables globales.

## Cambiar el archivo de ejemplo

En la función `main()` del archivo, las lineas `196` y `199` llevan las partes escenciales del directorio donde se encuentre el archivo. `DIR` señala la dirección de la carpeta mientras que `PUZZLE` nombra al archivo. Esto generará un output en un archivo además de lo mostrado en pantalla que se llamará: `out_{filename}`. Este archivo es para ser procesado debido a que tiene el formato: `TAM_POB:MAX_GEN:PROB_CRU:PROB_MUT:GEN:BESTGEN`

## Extra

El archivo Analisis Output es un archivo de Jupyter - Python3 donde se crearon los gráficos del informe.