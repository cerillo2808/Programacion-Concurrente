#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/**
 * @brief ...
 */
void* tirar_3veces (void* arg);

int main(int argc, char* argv[]) {

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

        double *resultado_equipo1;
        double *resultado_equipo2;

        for (int i = 0; i<cantidadAtletas; i++){

            int* datos1 = malloc(sizeof(int) * 2);
            int* datos2 = malloc(sizeof(int) * 2);

            // en datos se pasa el equipo y el numero de atleta.
            datos1[0] = 1; datos1[1] = i;
            datos2[0] = 2; datos2[1] = i;

            pthread_create(&atletas_equipo1[i], NULL, tirar_3veces, datos1);
            pthread_create(&atletas_equipo2[i], NULL, tirar_3veces, datos2);
        }

        for (int i = 0; i<cantidadAtletas; i++){
            pthread_join(atletas_equipo1[i], (void**)&resultado_equipo1);
            pthread_join(atletas_equipo2[i], (void**)&resultado_equipo2);
            
            if (*resultado_equipo1<*resultado_equipo2){
                ganes_equipo2++;
            } else if (*resultado_equipo1>*resultado_equipo2){
                ganes_equipo1++;
            }
            // si es empate, a nadie se le da punto.
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

        free(atletas_equipo1);
        free(atletas_equipo2);

        }

    return 0;
    } 

    void* tirar_3veces(void* arg){

        int* datos = (int*)arg;
        int equipo = datos[0];
        int indice = datos[1];

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

        printf("%d.%d: best shot put %f\n", equipo, indice + 1, *tiro_mas_alto);
        
        return tiro_mas_alto;
    }