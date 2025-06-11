# Programación Paralela y Concurrente
Estudiante: Liqing Yosery Zheng Lu C38680 <br>
Profesor: Daniel Alvarado <br>
Semestre I, 2025 <br>
Grupo 02

# Tabla de Contenidos
1. [Glossary](#glossary)
2. [Tareas](#tareas)
3. [Pthreads](#pthreads)
4. [OpenMP](#openmp)
5. [MPI](#mpi)

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
Un hilo de ejecución, también conocido como trabajadores o simplemente hilo, es un conjunyo de datos que el sistema operativo puede mandar al procesador y ejecutar. Un programa que solo tiene un hilo es secuencial, mientras que uno que tiene múltiples hilos permite la ejecución concurrente siempre y cuando la arquitectura de la computadora lo permita. 

### 7. Indeterminismo
El calendarizador define cuándo y dónde atender a los diferentes procesos. Las colas sólo se utilizan cuando el programa ya está en ejecución, pero esperando a alguna otra instrucción a ser procesada. El indeterminismo es cuando no podemos saber el orden exacto en que se procesan instrucciones por falta de control sobre el calendarizador. Más formalmente, es la incapacidad de conocer de forma precisa el orden de ejecución de los programas (hilos, procesos).

### 8. Memoria privada y compartida
Regiones continuas de memoria.

Privada: Es cuando la región de memoria es un struct y se le asigna a un hilo. Ningún hilo tiene acceso a la memoria privada de otros hilos, pero cada hilo tiene su propia memoria privada.

Compartida: Es cuando se define como un atributo dentro de la memoria privada. Todos los hilos pueden acceder a esta memoria.

### 9. Espera activa
También conocido como busywait. Es un ciclo que hace a un hilo de ejecución esperar repetitivamente hasta que una condición se haga falsa.

### 10. Condición de carrera
Modificación concurrente de memoria compartida. Los hilos compiten para editar la memoria que comparten entre ellos. Es una mala práctica de programación porque pueden generar resultados no predecibles e incorrectos. No es permitido para el curso, y para verificar que no hay condiciones de carrera se usa tsan. Los requisitos son:<br>
1. Concurrencia
2. Memoria compartida
3. Escritura en memoria

### 11. Control de concurrencia
Hay varios mecanismos para controlar la concurrencia. El concepto usualmente se basa en las colas que usa el sistema operativo como lista de espera para usar el procesador. Entre ellas está la exclusión mutua, también conocida como mutex.

### 12. Seguridad condicional
Es un mecanismo para controlar la memoria compartida. Formalmente, es cuando un hilo nunca toca la misma memoria de otro hilo. O sea, permite que cada hilo trabaje en diferentes partes de la memoria, evitando condiciones de carrera. Es una de las mejores formas para trabajar concurrencia porque no provoca un cuello de botella. 

### 13. Exclusión mutua
Conocida como mutex o candado. Es un mecanismo para controlar concurrencia, también es un tipo de espera inactiva. Tiene una región crítica, que son una o varias líneas de código que no permite que se ejecuten de manera paralela. Asegura que no existen condiciones de carrera dentro de la región crítica.

### 14. Semáforo
Según Dijkstra, un semáforo es un entero con signo que se puede incrementar con la instrucción "signal" y decrementar con "wait". Si el signo es negativo, el hilo espera para ejecutarse. Si el signo es positivo, el hilo puede continuar ejecutándose. Hay tres reglas:<br>
1. No se puede leer el valor actual del semáforo. Sólo se le puede incrementar o decrementar.
2. Cuando un hilo decrementa un semáforo, si el resultado es negativo, el hilo es bloqueado y no puede continuar hasta que otro hilo incremente el semáforo.
3. Los hilos desbloqueados por el semáforo siguen una tendencia indeterminística. No se sabe en qué orden los hilos van a ser desbloqueados.

### 15. Barrera
Mecanismo de sincronización para hacer que múltiples hilos lleguen a un mismo destino antes de continuar ejecutándose. Útil para procesos en donde hilos necesitan compartirse información pero aún no han terminado su tarea.

### 16. Variable de condición
Mecanismo de sincronización para que múltiples hilos esperen hasta que se cumpla una condición específica. Necesita ser utilizado con mútex para evitar condiciones de carrera.

### 17. Candado de lectura y escritura
Múltiples hilos pueden leer una misma variable a la vez, pero sólo una puede escribir a la vez. Esto evita condiciones de carrera, pero no limita tanto a la concurrencia porque permite la lectura.

### 18. Descomposición
Identificación de tareas que se pueden hacer de manera indendiente, y que por ende, de manera paralela. Tiene una granularidad, que es la cantidad y tamaño de las tareas. 
1. Granularidad fina: Hay muchas tareas pequeñas. Entre más trabajadores mejor, pero hay poca sincronización, poca comunicación.
2. Granularidad gruesa: Pocas tareas grandes. Mejora la sincronización, porque los trabajadores pueden comunicarse entre ellos más frecuentemente.

Hay diferentes técnicas para definir las unidades de descomposición.
1. Recursiva: El problema se divide en subproblemas que son procesadas de manera concurrente.
2. De datos: Se parte de manera equitativa el trabajo, el tiempo para conseguir la solución se vuelve más corta.
3. Exploratoria: Los trabajadores buscan resolver el mismo problema, pero de diferentes maneras. No todos los trabajadores van a contribuir a la solución, porque en cuanto un trabajador lo logre, avisa a los demás para que se detengan. El tiempo para conseguir la solución no es lineal con la cantidad de trabajadores.
4. Especulativa: Usando probabilidad, se procesa de manera concurrente la tarea actual y la tarea que se necesitará procesar en un futuro.

### 19. Mapeo
Técnica para descomponer el trabajo. La meta principal es asignar a cada trabajador una carga equitativa. Hay dos categorías prinicpales.
1. Estático: Conocido como por bloque. A cada trabajador le asigna un rango, o bloque, de trabajos que tiene que realizar. Estos rangos son fijos.
2. Dinámico: Se asigna la carga de trabajo en tiempo de ejecución. Toma en cuenta de que no todas las tareas toman la misma cantidad de tiempo. Cuando un trabajador se desocupa, empieza la siguiente tarea a realizar.

### 20. Incremento de velocidad (speedup)
La división del tiempo serial entre el tiempo paralelo. Es la razón para saber qué tan más acelerado es la versión paralela. Sólo considera los tiempos de ejecución, no los recursos que se utilizaron.

### 21. Eficiencia
Es el speedup, pero considera los recursos que se utilizaron para lograr un mejor desempeño. Siempre es un valor entre 0 (no eficiente). y 1 (muy eficiente). Se calcula como speedup entre cantidad de hilos.

### 22. Comunicación punto a punto entre procesos
Método de intercambio de datos directamente entre dos procesos. Puede lograrse con mensajes o buffers compartidos.

### 23. Comunicación colectiva entre procesos

### 24. Reducción

# Tareas
| # | Nombre | Descripción
|---|---|---
| 1 | [serial](homeworks/serial)| Simulación de calor
| 2 | [pthread](homeworks/pthread) | Procesamiento paralelo de la simulación de calor
| 3 | [optimized](homeworks/optimized/) | Intentos de optimización con el reporte del proceso
| 4 | [omp_mpi](homeworks/omp_mpi/) | Comparación de la versión más optimizada en OpenMP contra MPI

# Pthreads
| # | Semana | Tipo | Nombre | Descripción
|---|---|---|---|---
| 1 | 1 | Ejemplo | [hello](examples/pthreads/hello/) | Main crea un hilo secundario que imprime hello.
| 2 | 1 | Ejercicio | [recursive_hello](exercises/pthreads/recursive_hello/) | El método create_thread se llama a sí mismo para crear más hilos. La recursión se controla con el controlador, que es el input del usuario.
| 3 | 2 | Ejemplo | [hello_w](examples/pthreads/hello_w/) | Indeterminismo. Los hilos no siempre se ejecutan en el orden en el que fueron creados.
| 4 | 2 | Ejercicio | [grandma_lottery](exercises/pthreads/grandma_lottery/) | Main espera a dos hilos con pthread_join. Simulación de la abuela siendo main y sus nietos siendo los hilos.
| 5 | 3 | Ejemplo | [hello_iw_pri](examples/pthreads/hello_iw_pri/) | Los hilos usan memoria privada para saber cuál número de hilo son.
| 6 | 3 | Ejemplo | [hello_iw_shr](examples/pthreads/hello_iw_shr/) | Los hilos usan memoria compartida para saber cuál es la cantidad total de hilos.
| 7 | 3 | Ejemplo | [hello_order_busywait](examples/pthreads/hello_order_busywait/) | Se usa la espera activa para que los hilos se impriman en orden.
| 8 | 3 | Ejercicio | [team_shot_put](exercises/pthreads/team_shot_put/) | Main usa pthread_join para esperar a todos los hilos que crea. Cada hilo retorna un valor, y cuando regresan todos los hilos, los valores se comparan entre ellos. Simulación del árbitro siendo main y los jugadores siendo los hilos.
| 9 | 3| Ejercicio | [create_thread_team](exercises/pthreads/create_thread_team/) | Se usan los métodos create_threads y join_threads para modularizar la creación y espera de hilos.
| 10 | 4 | Ejercicio | [delayed_busy_wait](exercises/pthreads/delayed_busy_wait/) | Modificación de hello_order_busywait. Se usa un delay para la espera activa.
| 11 | 4 | Ejemplo | [race_position](examples/pthreads/race_position/) | Modificación de hello_order_busywait. Se usa el mutex para que los hilos modifiquen a una misma variable compartida.
| 12 | 4 | Ejercicio | [birthday_pinata](exercises/pthreads/birthday_pinata/) | Se usa un mutex para controlar que sólo un hilo a la vez modifique memoria compartida. Simulación del hilo principal siendo la piñata y los hilos como los que celebran el cumpleaños.
| 13 | 4 | Ejemplo | [hello_order_semaphor](examples/pthreads/hello_order_semaphor/) | Modificación de hello_order_busywait, donde se usa un arreglo de semáforos para controlar la concurrencia.
| 14 | 4 | Ejemplo | [hello_order_cond_safe](examples/pthreads/hello_order_cond_safe/) | Modificación de hello_order_busywait, donde se usa el concepto de seguridad condicional para evitar escritura en memoria compartida.
| 15 | 4 | Ejercicio | [sem_vs_condsafe](exercises/pthreads/safe_vs_condsafe/) | Comparación de semáforos contra seguridad condicional.
| 16 | 4 | Ejercicio | [building_tasks](exercises/pthreads/building_tasks/) | Hilos que tienen diferentes dependencias.
| 17 | 5 | Ejemplo | [prod_cons_bound](examples/pthreads/prod_cons_bound/) | Productor y consumidor regulan sus acciones a través de semáforos. El buffer que tienen es limitado.
| 18 | 5 | Ejemplo | [prod_cons_unbound](examples/pthreads/prod_cons_unbound/) | El buffer se asume como ilimitado.
| 19 | 6 | Taskc | [network_simul_packet_loss](taskc/network_simul_packet_loss/) | Simulación de red con pérdida de paquetes.
| 20 | 6 | Taskc | [network_simul_packet_loss2](taskc/network_simul_packet_loss2/) | Modificación de network_simul_packet_loss con porcentaje de pérdida afectando a todos los paquetes.
| 21 | 6 | Taskc | [network_simul_producers](taskc/network_simul_packet_loss2/) | Modificación de network_simul_packet_loss para que el usuario pueda ingresar cantidad de productores que trabajan concurrentemente.
| 22 | 6 | Taskc | [network_simul_bounded](taskc/network_simul_bounded/) | Modificación de network_simul_packet_loss para que el usuario pueda introducir la máxima cantidad de paquetes en cada cola.
| 23 | 8 | Ejercicio | [signaling_3](exercises/pthreads/signaling_3/) | Red de petri y pseudocódigo para que tres hilos se ejecuten en un orden específico.
| 24 | 8 | Ejercicio | [taskc_patterns](exercises/pthreads/taskc_patterns/README.md) | Patrones básicos de concurrencia de tareas.
| 25 | 9 | Common | [ComandosCluster](common/ComandosCluster.md) | Comandos básicos para usar el clúster Poás.

# OpenMP
| # | Semana | Tipo | Nombre | Descripción
|---|---|---|---|---
| 1 | 9 | Ejemplo | [several_for_stages](examples/openmp/several_for_stages/) | El usuario pueda indicar la cantidad de etapas como tercer argumento de línea de comandos.
| 2 | 10 | Ejemplo | [shuffle_sentence](examples/openmp/shuffle_sentence/) | Se controla el orden en que se ejecutan tasks de omp.
| 3 | 9 | Ejemplo | [omp_team](examples/openmp/omp_team/) | Equipo de hilos secundarios saludan concurrentemente con OpenMP.
| 4 | 9 | Ejemplo | [omp_for](examples/openmp/omp_for/) | Comparación de diferentes mapeos.

# MPI
| # | Semana | Tipo | Nombre | Descripción
|---|---|---|---|---
| 1 | 11 | Ejemplo | [hello](examples/mpi/hello/) | Hello usando MPI y ejecutándolo desde el clúster Poás.
| 2 | 12 | Ejercicio | [mpi_ring](exercises/mpi/mpi_ring/) | Usar comunicación punto a punto con MPI
| 3 | 12 | Ejercicio | [hybrid_distr_arg](exercises/mpi/hybrid_distr_arg/) | Distribuye el rango del input entre los procesos de manera equitativa.
| 4 | 13 | Ejercicio | [send_recv_ord_itm](exercises/mpi/send_recv_ord_itm/) | Seguridad condicional con comunicación punto a punto.
| 5 | 13 | Ejercicio | [send_recv_ord_sem](exercises/mpi/send_recv_ord_sem/) | Saludar determinísticamente.