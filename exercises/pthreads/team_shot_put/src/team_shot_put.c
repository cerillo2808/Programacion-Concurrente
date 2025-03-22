#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 * @brief ...
 */
void* tirar_3veces ();

int main(int argc, char* argv[]) {

    // para que no tire warning de que no se usa argc
    (void) argc;

    // para verificar que sí haya parámetros
    if (argc < 2) {
        printf("Error: debe pasar la cantidad de atletas como argumento\n");
        // se termina el programa con error
        return 1;
    }
    
    // convertir el string del input a int
    int cantidadAtletas = atoi(argv[1]);

    // si el input es par, da error
    if (cantidadAtletas%2==0){
        printf("Error: número par de atletas\n");
        // se termina el programa con error
        return 1;

    } else{
        // no hay errores y se sigue

        // son dos equipos, cada equipo con una cantidad impar de atletas
        double pizarra[2][cantidadAtletas];

        // instancio a los atletas
        pthread_t *atletas = malloc(sizeof(pthread_t)*cantidadAtletas*2);

        for (int i =0; i<2; i++){
            for (int j = 0; j<cantidadAtletas; j++){

                // hacer uno único para cada atleta
                int indice = i*cantidadAtletas+j;

                pthread_create(&atletas[indice], NULL, tirar_3veces, NULL);
                double *resultado;
                pthread_join(atletas[indice], (void**)&resultado);
                pizarra[i][j] = *resultado;
                free(resultado);
            }
        }

        for (int equipo = 0; equipo<2; equipo++){
            for (int atleta = 0; atleta<cantidadAtletas; atleta++){
                printf("%f\n", pizarra[equipo][atleta]);
            }
        }

    }

    return 0;
    } 

    void* tirar_3veces(){
        unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self();
        double *tiro = malloc(sizeof(double));
        *tiro = ((double)rand_r(&seed) / RAND_MAX) * 20.5;
        return tiro;

        // TO-DO: Hacer que tire tres veces y retorne el más alto.
    }