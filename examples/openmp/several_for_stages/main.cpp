// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, std::cout)
  {
    #pragma omp for
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    #pragma omp critical(stdout)
      std::cout << "stage 1: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    }

    #pragma omp single
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
    // #pragma omp barrier

    #pragma omp for
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      #pragma omp critical(stdout)
      std::cout << "stage 2: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    }

    #pragma omp single
    std::cout << /*omp_get_thread_num() <<*/ std::endl;
    // #pragma omp barrier

    #pragma omp for
    for (int iteration = 0; iteration < iteration_count; ++iteration) {
      #pragma omp critical(stdout)
      std::cout << "stage 3: " << omp_get_thread_num() << '/'
        << omp_get_num_threads() << ": iteration " << iteration << '/'
        << iteration_count << std::endl;
    }
  }
}