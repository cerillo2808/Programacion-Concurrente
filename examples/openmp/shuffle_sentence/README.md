# Enunciado

Escriba un programa que lea palabras de la entrada estándar hasta que estas se acaben. Piense que cada palabra es una tarea y las tareas deben hacerse de forma concurrente. El objetivo con cada tarea es simplemente imprimir la palabra en la salida estándar, separada de otras por un espacio en blanco, de manera que se forme una oración con indeterminismo (con sentido o no). Su programa debe crear tareas de OpenMP que respeten estas reglas:

    La primera palabra leída de la entrada estándar debe siempre imprimirse de primero.

    Las demás palabras se imprimen indeterminísticamente en cualquier orden separadas por un espacio en blanco.

    La oración siempre debe terminar en punto.

Todas las tres reglas anteriores deben ser realizadas por tareas de OpenMP. Ejemplos de ejecución:

````
$ echo El niño pobre | bin/shuffle_sentence
El pobre niño.

$ echo El del niño mordió al papá perro | bin/shuffle_sentence
El papá mordió al perro del niño.

$ echo El del niño mordió al papá perro | bin/shuffle_sentence
El mordió perro al papá niño del.
````

# Mini investigación

Cómo hacer que leer la palabra sea el primer task que se ejecute y que escribir el punto siempre sea el último task que se ejecute.

## Resultados
TODO: Usar un vector