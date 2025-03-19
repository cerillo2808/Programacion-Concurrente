# Enunciado
Escriba un programa que al correr cree dos hilos, el principal y uno secundario. Ambos hilos deben saludar en la salida estándar con el mensaje "Hello from main thread" y "Hello from secondary thread" respectivamente. Utilice un analizador dinámico de código como las herramientas memcheck y helgrind de Valgrind, o los Google Sanitizers, para asegurarse de que su programa no provoca condiciones de carrera.

# Comentarios 
- Los include van en orden alfabético por el linter.
- Pasos para crear un hilo: <br>
1. Se instancia el hilo con la instrucción `pthread_t thread`
2. Se crea el hilo con la instrucción `pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL)` <br> Tómese en cuenta de que el primer parámetro es la dirección de memoria de la instancia del hilo. El tercer parámetro es la función que se quiere que haga el nuevo hilo.
3. Verificar que el hilo sí se abrió comparando `error == EXIT_SUCCESS`. Nota: error es el entero que `pthread_create` devuelve.