#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
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
            printf("Archivo %s abierto correctamente. Cargando información...\n", jobPath);
            // TO-DO: leer cada linea del job e ir inicializando el objeto plate.
        }
        
    }
    return 0;
}