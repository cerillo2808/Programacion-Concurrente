# Programación Paralela y Concurrente
Estudiante: Liqing Yosery Zheng Lu C38680 <br>
Profesor: Daniel Alvarado <br>
Semestre I, 2025 <br>
Grupo 02


# Glossary

### 1. Programación serial
También conocido como secuencial, es donde las instrucciones se ejecutan sólo cuando la anterior ha sido terminada.

### 2. Programación concurrente
Todo lo que no es serial es concurrente. Las instrucciones no tienen que esperar a que la anterior termine de ejecutarse para empezar. Por ejemplo, el cambio de contexto es cuando las instrucciones son realizadas de manera intercalada. 

### 3. Programación paralela
Una forma de concurrencia. Las instrucciones se ejecutan al mismo tiempo. Depende del hardware porque para poder ejecutar instrucciones al mismo tiempo, se deben de hacer en diferentes núcleos.

### 4. Concurrencia de tareas
Relacionado con la separación de asuntos. Diferentes tareas son realizadas por su respectivo "especialista", o hilo. Por ejemplo, en un reproductor de video un hilo consume bytes de internet, mientras que otro hilo renderiza estos bytes que el hilo anterior produjo.

### 5. Paralelismo de datos
Relacionado con el incremento del desempeño. En vez de hacer que un solo hilo se encargue de todo el conjunto de datos a procesar, se divide la carga en varios hilos.

### 6. Hilo de ejecución
Un hilo de ejecución, también conocido como trabajadores o simplemente hilo, es un arreglo de valores o proceso que el sistema operativo puede mandar al procesador y ejecutar. Un programa que solo tiene un hilo es secuencial, mientras que uno que tiene múltiples hilos permite la ejecución concurrente siempre y cuando la arquitectura de la computadora lo permita.

### 7. Indeterminismo
El calendarizador define cuándo y dónde atender a los diferentes procesos. Las colas sólo se utilizan cuando el programa ya está en ejecución, pero esperando a alguna otra instrucción a ser procesada. El indeterminismo es cuando no podemos saber el orden exacto en que se procesan instrucciones por falta de control sobre el calendarizador. Más formalmente, es la incapacidad de conocer de forma precisa el orden de ejecución de los programas (hilos, procesos).

### 8. Memoria privada y compartida
Regiones continuas de memoria.

Privada: Es cuando la región de memoria es un struct y se le asigna a un hilo. Ningún hilo tiene acceso a la memoria privada de otros hilos, pero cada hilo tiene su propia memoria privada.

Compartida: Es cuando se define como un atributo dentro de la memoria privada. Todos los hilos pueden acceder a esta memoria.

### 9. Espera activa
También conocido como busywait. Es un ciclo que hace a un hilo de ejecución esperar repetitivamente hasta que una condición se haga falsa.

### 10. Condición de carrera

### 11. Control de concurrencia

### 12. Seguridad condicional

### 13. Exclusión mutua

### 14. Semáforo

### 15. Barrera

### 16. Variable de condición

### 17. Candado de lectura y escritura

### 18. Descomposición

### 19. Mapeo

### 20. Incremento de velocidad (speedup)

### 21. Eficiencia

### 22. Comunicación punto a punto entre procesos

### 23. Comunicación colectiva entre procesos

### 24. Reducción
