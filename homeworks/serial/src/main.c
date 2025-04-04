// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <controlador.h>

int main(int argc, char *argv[]) {
    // asegurar de que entren dos inputs del usuario
    if (verificar_argumentos(argc, argv)) {
        // retorno de 1 significa error
        return 1;
    }

    printf("Fin del programa\n"); //NOLINT

    return 0;
}
