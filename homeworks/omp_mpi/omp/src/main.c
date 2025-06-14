// Copyright [2025] <Liqing Yosery Zheng Lu>

#include <stdio.h>
#include <stdlib.h>
#include <controlador.h>

int main(int argc, char *argv[]) {
    // Asegurar de que entren dos inputs del usuario
    if (run(argc, argv)) {
        return 1;
    }

    printf("Fin del programa\n"); //NOLINT

    return 0;
}
