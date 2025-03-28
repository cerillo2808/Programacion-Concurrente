#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <plate.h>
#include <inttypes.h>
#include <time.h>
#include <controlador.h>
#include <sys/stat.h>
#include <sys/types.h>

int verificar_argumentos(int argc, char *argv[]){

    if(argc>3){
        // muchos argumentos
        printf("Error: Hay más argumentos de los necesarios. Ingrese la dirección del archivo y la cantidad de hilos a utilizar.\n");
        return 1;

    } else if(argc<=1){
        // faltan argumentos
        printf("Error: Hay menos argumentos de los necesarios. Ingrese la dirección del archivo y la cantidad de hilos a utilizar.\n");;
        return 1;

    } else{
        // hay al menos un argumento
        // TO-DO: verificar que se haya ingresado cantidad hilos

        char *jobPath = argv[1];

        printf("Intentando abrir: '%s'\n", jobPath);
        FILE *jobFile = fopen(jobPath, "r");
        // el parámetro r es para lectura (read)

        if (jobFile==NULL){
            printf("Error: No se pudo abrir. Hay un error con el nombre o path de su archivo.\n");
            return 1;

        } else{
            printf("Archivo %s abierto correctamente. Cargando información.\n", jobPath);

            char nombreJob[256];
            
            // buscar el / del job
            char *base = strrchr(jobPath, '/');
            if (base) {
                // saltarse el /
                base++;
            } else {
                base = jobPath;
            }

            strncpy(nombreJob, base, sizeof(nombreJob));

            // Eliminar la extensión .txt
            char *punto = strrchr(nombreJob, '.');
            if (punto && strcmp(punto, ".txt") == 0) {
                // el punto se vuelve caracter nulo
                *punto = '\0';
            }

            char linea[256];
            // buffer para cada linea de job, máximo de 256 chars

            while (fgets(linea, sizeof(linea), jobFile)) {
                // crear un plate para cada linea del txt
                if(crear_plate(linea, nombreJob)){
                    return 1;
                }
            }
        
            fclose(jobFile);
        }
        
    }
    return 0;
}

void cambio_temperatura(uint64_t* temperaturas, Plate plate){
    
    int iteraciones;
    double cambio_maximo;
    double cambio;

    // Se crea matriz temporal para calcular el cambio
    uint64_t *temperaturas_temporal = (uint64_t *)malloc(plate.R * plate.C * sizeof(uint64_t));
    if (!temperaturas_temporal) {
        printf("Error: No se pudo asignar memoria para la matriz temporal.\n");
        return;
    }

    clock_t tiempoInicio = clock();

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
                    uint64_t arriba = temperaturas[(i - 1) * plate.C + j];
                    uint64_t abajo = temperaturas[(i + 1) * plate.C + j];
                    uint64_t izquierda = temperaturas[i * plate.C + (j - 1)];
                    uint64_t derecha = temperaturas[i * plate.C + (j + 1)];

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

    // calcular el tiempo transcurrido
    clock_t tiempoFin = clock();
    double tiempoSegundos = (double)(tiempoFin - tiempoInicio) / CLOCKS_PER_SEC;
    
    printf("Difusión completada.\n");

    generar_archivo_binario(plate.nombreArchivo, plate.R, plate.C, temperaturas);
    
    char nombre_final_tsv[512];
    snprintf(nombre_final_tsv, sizeof(nombre_final_tsv), "%s%s", plate.nombreJob, ".tsv");
    generar_archivo_tsv("output", nombre_final_tsv, plate, tiempoSegundos, iteraciones);
    
    free(temperaturas_temporal);
}

// Método copiado de la página del profesor Jeisson Hidalgo
// Return parameter text must have at least 48 chars (YYYY/MM/DD hh:mm:ss)
char* format_time(const time_t seconds, char* text, const size_t capacity) {
    const struct tm* gmt = gmtime(&seconds);
    snprintf(text, capacity, "%04d/%02d/%02d\t%02d:%02d:%02d", gmt->tm_year - 70,
        gmt->tm_mon, gmt->tm_mday - 1, gmt->tm_hour, gmt->tm_min, gmt->tm_sec);
    return text;
}

void generar_archivo_binario(const char *nombre_archivo, uint64_t R, uint64_t C, uint64_t *temperaturas) {
    char rutaCompleta[512];

    // Crear el directorio "output" si no existe
    mkdir("output", 0777);

    // Construir la ruta completa del archivo dentro de "output"
    snprintf(rutaCompleta, sizeof(rutaCompleta), "output/%s", nombre_archivo);

    FILE *archivo = fopen(rutaCompleta, "wb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo bin para escritura.\n");
        return;
    }

    fwrite(&R, sizeof(uint64_t), 1, archivo);  // Guardar R
    fwrite(&C, sizeof(uint64_t), 1, archivo);  // Guardar C
    fwrite(temperaturas, sizeof(uint64_t), R * C, archivo);  // Guardar la matriz de temperaturas

    fclose(archivo);
}

void generar_archivo_tsv(const char *directorio, const char *nombreArchivo, Plate plate, double tiempoSegundos, int iteraciones) {
   
    char rutaCompleta[512];
    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s/%s", directorio, nombreArchivo);

    FILE *tsvFile = fopen(rutaCompleta, "a");
    if (!tsvFile) {
        printf("Error: No se pudo abrir el archivo TSV para escritura en %s\n", rutaCompleta);
        return;
    }

    char tiempo[512];
    format_time((time_t)tiempoSegundos, tiempo, sizeof(tiempo));

    // Escribir los resultados en el archivo TSV
    fprintf(tsvFile, "%s\t%ld\t%ld\t%ld\t%ld\t%d\t%s\n", plate.nombreArchivo, plate.delta, plate.alfa, plate.h, plate.epsilon, iteraciones, tiempo);

    fclose(tsvFile);
}

