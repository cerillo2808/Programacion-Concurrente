# Enunciado
Modifique el programa de "hola mundo" para crear una cantidad arbitraria de hilos de ejecución. Cada uno de los cuales imprime "Hello from secondary thread i" en la salida estándar y termina su ejecución. Permita al usuario indicar la cantidad de hilos de ejecución como argumento de la línea de comandos. Si este argumento se omite, suponga la cantidad de CPUs disponibles en el sistema.

# Observaciones
- Los hilos creados en el for se ejecutan en orden no predecible por el indeterminismo.
- Si en pthread_create se le pasa un cuarto parámetro (los argumentos), funciona como pasarle un parámetro a la función que ejecuta el hilo.
- Hay que liberar la memoria de los hilos porque estos fueron llamados con malloc.