// Copyright 2024 ECCI-UCR CC-BY 4.0
#include <omp.h>
#include <iostream>

bool do_heavy_work() {
  return true;
}

int main(int argc, char* argv[]) {
  const int thread_count = argc >= 2 ? ::atoi(argv[1]) : 1;
  const int iteration_count = argc >= 3 ? ::atoi(argv[2]) : thread_count;

  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(iteration_count, thread_count, std::cout)
  for (int iteration = 0; iteration < iteration_count; ++iteration) {
    if (!do_heavy_work()) {
      // break;
    }
    #pragma omp critical(print)
    std::cout << omp_get_thread_num() << '/' << thread_count
      << ": iteration " << iteration << '/' << iteration_count << std::endl;
  }
}
