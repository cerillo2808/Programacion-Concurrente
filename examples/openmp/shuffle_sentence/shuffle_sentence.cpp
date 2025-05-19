// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <sstream>

// TODO(you): use for convenience or remove this procedure
void say(const std::string& text) {
  std::ostringstream buffer;
  buffer << omp_get_thread_num() << "/" << omp_get_num_threads()
      << ": " << text << std::endl;
  std::cout << buffer.str();
}

int main(int argc, char* argv[]) {
  const int thread_count = argc >= 2 ? atoi(argv[1]) : omp_get_max_threads();
  std::string word;

  #pragma omp parallel num_threads(thread_count) default(none) private(word) shared(std::cin, std::cout)
    {
        // Sólo un hilo hace todas las tasks. Estas tasks van a una cola.
        #pragma omp single
        {
            // TODO(you): First task: read and print first word of the sentence
            #pragma omp task depend(out: word)
            {
                std::cin >> word;
                std::cout << word;
            }

            // TODO(you): Second task: shuffle remaining words of the sentence
            #pragma omp task
            {
                // TODO(you): read and print the rest of the sentence
                while (std::cin >> word) {
                    std::cout << ' ' + word;
                }
            }

            // TODO(you): Third task: finish the sentence
            #pragma omp task final(true)
            {
                std::cout << '.' << std::endl;
            }
        }
        // Cuando el hilo llega hasta acá, ve si hay más tasks en la cola lo hace.
    }
}