#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>

int verificar_argumentos(int argc, char *argv[]){
    if(argc>3){
        // muchos argumentos
        printf("Hay más argumentos de los necesarios. Ingrese la dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 1;

    } else if(argc<=2){
        // faltan argumentos
        printf("Hay menos argumentos de los necesarios. Ingrese la dirección del archivo y la cantidad de hilos a utilizar.\n");;
        return 1;

    } else{
        // son 2 argumentos

        char *jobPath = argv[1];

        printf("Intentando abrir: '%s'\n", jobPath);
        FILE *jobFile = fopen(jobPath, "r");
        // el parámetro r es para lectura (read)

        if (jobFile==NULL){
            printf("No se pudo abrir. Hay un error con el nombre o path de su archivo.\n");
            return 1;

        } else{
            printf("Archivo %s abierto correctamente. Cargando información.\n", jobPath);

            char linea[256];
            // buffer para cada linea de job, máximo de 256 chars

            while (fgets(linea, sizeof(linea), jobFile)) {
                // crear un plate para cada linea del txt
                if(crear_plate(linea)){
                    return 1;
                }
            }
        
            fclose(jobFile);
        }
        
    }
    return 0;
}

void cambio_temperatura(double* temperaturas, Plate plate){
    
    int iteraciones;
    double cambio_maximo;
    double cambio;

    // Se crea matriz temporal para calcular el cambio
    double *temperaturas_temporal = (double *)malloc(plate.R * plate.C * sizeof(double));
    if (!temperaturas_temporal) {
        printf("Error: No se pudo asignar memoria para la matriz temporal.\n");
        return;
    }

    do{
        cambio_maximo = 0.0;
        for (uint64_t i = 0; i<plate.R; i++){
            for (uint64_t j = 0; j<plate.C; j++){
                uint64_t indice = i * plate.C + j;

                if (i == 0 || i == plate.R - 1 || j == 0 || j == plate.C - 1){
                    // son los bordes de la matriz, no se cambian
                    temperaturas_temporal[indice] = temperaturas[indice];
                    
                } else{
                    // no son bordes y tienen que ser transformadas
                    double arriba = temperaturas[(i - 1) * plate.C + j];
                    double abajo = temperaturas[(i + 1) * plate.C + j];
                    double izquierda = temperaturas[i * plate.C + (j - 1)];
                    double derecha = temperaturas[i * plate.C + (j + 1)];

                    // nueva temperatura se calcula con la fórmula y se pone en la matriz temporal
                    temperaturas_temporal[indice] = temperaturas[indice] + plate.alfa * plate.delta * ((arriba + abajo + izquierda + derecha - 4.0 * temperaturas[indice]) / (plate.h * plate.h));
                    
                    // verificar y actualizar cambio máximo
                    cambio = temperaturas_temporal[indice] - temperaturas[indice];

                    if (cambio < 0){
                        // si es negativo, guardarlo como positivo
                        cambio = -cambio;
                    } 

                    if (cambio > cambio_maximo){
                        cambio_maximo = cambio;
                    }
                }    
            }
        }

        // sobreescribir viejas temperaturas con las nuevas
        for (uint64_t i = 0; i < plate.R * plate.C; i++) {
            temperaturas[i] = temperaturas_temporal[i];
        }

        iteraciones++;
    } while(cambio_maximo > plate.epsilon);
    
    printf("Difusión completada");
    
    free(temperaturas_temporal);
}