// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

/**
 * @brief ...
 */
void* greet(void* data);

// procedure main:
int main(void) {
    size_t variable_local = 2;
  // create_thread(greet)
  pthread_t thread;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ &variable_local);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread. Counter: %zu\n", variable_local);
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

// procedure greet:
void* greet(void* data) {
    size_t variable_local = *((size_t*)data);
  //(void)data;
  // print "Hello from secondary thread"
  if (variable_local==0){
    printf("Goodbye. Counter: %zu\n", variable_local);
  }
  else{
    printf("No es cero. Creando otro hilo.\n");
    variable_local= variable_local-1;
    pthread_t thread;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ &variable_local);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from another thread. Counter: %zu\n", variable_local);
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    fprintf(stderr, "Error: could not create another thread\n");
  }
  return NULL;
  }
  
  return NULL;
}  // end procedure