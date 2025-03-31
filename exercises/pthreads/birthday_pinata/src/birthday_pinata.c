#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief ...
 */
pthread_t* create_threads(size_t count, void*(*routine)(void*), void* data);
int join_threads(size_t count, pthread_t* threads);
int pegar_pinata(int golpes_maximos);

int main(int argc, char *argv[]) {
    if (argc<3){
        printf("Ingrese la cantidad de hilos y la dureza de la piñata.\n");
        return 1;
    }

    int hilos = argc[2];
    int golpes_maximos = argc[3];

    pthread_t* threads = create_threads(hilos, pegar_pinata(golpes_maximos), void);
    join_threads(golpes_maximos, &threads);

    printf("Thread %d/%d: %d hits\n");

    return 0;
}

int pegar_pinata(int golpes_maximos){
    // hacer random de golpes que le puede dar
    return 0;
}
 
pthread_t* create_threads(size_t count, void*(*routine)(void*), void* data) {
    pthread_t* threads = (pthread_t*) calloc(count, sizeof(pthread_t));
    if (threads) {
        for (size_t index = 0; index < count; ++index) {
        if (pthread_create(&threads[index], NULL, routine, data) != 0) {
            fprintf(stderr, "error: could not create thread %zu\n", index);
            join_threads(index, threads);
            return NULL;
        }
        }
    }
    return threads;
}

int join_threads(size_t count, pthread_t* threads) {
    int error_count = 0;
    for (size_t index = 0; index < count; ++index) {
        const int error = pthread_join(threads[index], NULL);
        if (error) {
        fprintf(stderr, "error: could not join thread %zu\n", index);
        ++error_count;
        }
    }
    free(threads);
return error_count;
}