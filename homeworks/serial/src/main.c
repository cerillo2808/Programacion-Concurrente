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
        // TO-DO: verificar que el archivo abre
        printf("Cargando información...\n");
    }
    return 0;
}