#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 * @brief ...
 */
 void* comprar_numero ();

int main () {

    // preparar variables para los números de los nietos
    unsigned int* numero_nieto1;
    unsigned int* numero_nieto2;

    // instanciar nietos
    pthread_t nieto1;
    pthread_t nieto2;

    // crear a los nietos como hilos
    pthread_create(&nieto1, NULL, comprar_numero, NULL);
    pthread_create(&nieto2, NULL, comprar_numero, NULL);

    // join, hacer que la abuela espere por sus nietos. Los numeros que devuelve el comprar_numero llegan a las variables inicializadas al principio del main.
    pthread_join(nieto1, (void**)&numero_nieto1);
    pthread_join(nieto2, (void**)&numero_nieto2);

    //imprimir los números
    printf("%u\n", *numero_nieto1);
    printf("%u\n", *numero_nieto2);

    //eliminar nietos
    free(numero_nieto1);
    free(numero_nieto2);

    return 0;

}

void* comprar_numero (/*void* seed*/){
    /*unsigned int* s = (unsigned int*) seed;
    unsigned int *loteria = malloc(sizeof(unsigned int));
    *loteria = rand_r(s) % 101;
    return loteria;*/

    unsigned int *loteria = malloc(sizeof(unsigned int));
    *loteria = 3;
    return loteria;

}