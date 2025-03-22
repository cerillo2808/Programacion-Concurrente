#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 * @brief ...
 */
void* tirar_3veces (void* arg);

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
        int ganes_equipo1 = 0;
        int ganes_equipo2 = 0;

        // instancio a los atletas
        pthread_t *atletas_equipo1 = malloc(sizeof(pthread_t)*cantidadAtletas);
        pthread_t *atletas_equipo2 = malloc(sizeof(pthread_t)*cantidadAtletas);

        for (int i = 0; i<cantidadAtletas; i++){

            double *resultado_equipo1;
            double *resultado_equipo2;
            int* indice1 = malloc(sizeof(int));
            int* indice2 = malloc(sizeof(int));
            *indice1 = i;
            *indice2 = i;

            pthread_create(&atletas_equipo1[i], NULL, tirar_3veces, indice1);
            pthread_create(&atletas_equipo2[i], NULL, tirar_3veces, indice2);

            pthread_join(atletas_equipo1[i], (void**)&resultado_equipo1);
            printf("1.%d: best shot put %f\n", i+1, *resultado_equipo1);
            pthread_join(atletas_equipo2[i], (void**)&resultado_equipo2);
            printf("2.%d: best shot put %f\n", i+1, *resultado_equipo2);
            
            if (*resultado_equipo1<*resultado_equipo2){
                ganes_equipo2++;
            } else if (*resultado_equipo1>*resultado_equipo2){
                ganes_equipo1++;
            }
            // si es empate, a nadie se le da punto.
            
            free(indice1);
            free(indice2);

        }

        // Imprimir resultados finales
        printf("\nGanes equipo 1: %d\n", ganes_equipo1);
        printf("Ganes equipo 2: %d\n", ganes_equipo2);
        
        if (ganes_equipo1<ganes_equipo2){
            printf("\nGana equipo 2\n");
        } else if (ganes_equipo1>ganes_equipo2){
            printf("\nGana equipo 1\n");
        } else{
            printf("\nEmpate\n");
        }

        }

    return 0;
    } 

    void* tirar_3veces(void* arg){

        int indice = *((int*)arg);
        free(arg);

        unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ indice;
        // la seed depende del índice que se pasó como parámetro

        double *tiro_mas_alto = malloc(sizeof(double));
        double tiro_actual; 
        for (int i = 0; i<3; i++){
           tiro_actual = ((double)rand_r(&seed) / RAND_MAX) * 20.5;
           if (tiro_actual > *tiro_mas_alto) {
            *tiro_mas_alto = tiro_actual;
            }  
        }
        
        return tiro_mas_alto;
    }