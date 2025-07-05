#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* hydrogen(void* arg);
void* oxygen(void* arg);
void bond();

int main() {
    char atom;
    pthread_t thread;
    
    while (scanf(" %c", &atom) == 1) {
        switch (atom) {
            case 'H':
                pthread_create(&thread, NULL, hydrogen, NULL);
                pthread_detach(thread);
                break;
            case 'O':
                pthread_create(&thread, NULL, oxygen, NULL);
                pthread_detach(thread);
                break;
            default:
                printf("Átomo desconocido: %c\n", atom);
                break;
        }
    }
    
    // Esperar un poco para que terminen los hilos
    sleep(1);
    return 0;
}

void* hydrogen(void* arg) {
    (void)arg;
    bond();
    return NULL;
}

void* oxygen(void* arg) {
    (void)arg;
    bond();
    return NULL;
}

void bond() {
    printf("Átomo se enlaza\n");
}
