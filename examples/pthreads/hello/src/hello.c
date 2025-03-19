// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
// los include usualmente van en orden alfabético por el linter

/**
 * @brief ...
 */
void* greet(void* data);

// procedure main:
int main(void) {
  // create_thread(greet)
  pthread_t thread;
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
  // el método a realizar por el nuevo hilo está en el tercer parámetro
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");
    pthread_join(thread, /*value_ptr*/ NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

// procedure greet:
void* greet(void* data) {
  (void)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread\n");
  return NULL;
}  // end procedure
