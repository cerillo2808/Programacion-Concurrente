#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    
    
    
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