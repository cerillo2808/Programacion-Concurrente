# Enunciado

Espera activa con retraso. ¿Se corrige el problema de la espera activa si en lugar de tener un ciclo vacío, se espera un cierto tiempo? Copie su carpeta ejemplos/pthreads/hello_order_busywait a ejercicios/pthreads/delayed_busy_wait. Permita que el usuario pueda invocar su programa dos argumentos de línea de comandos: la cantidad de hilos a crear, y la cantidad de microsegundos a esperar cuando no es el turno del hilo de ejecución.

Espera activa con retraso constante. Si no es el turno de un hilo, éste espera una cantidad constante de microsegundos, algo como:

`// Constant delayed busy waiting: wait until it is my turn`<br>
`while (next_thread < my_thread_id) {
  usleep(delay);
}`

Recuerde probar la calidad de su código (sanitizers, linter). Luego ejecute al menos tres veces su solución (sin sanitizers) con la cantidad máxima de hilos de ejecución que su sistema operativo le permite crear y una espera de 50µs. Anote la mayor de las duraciones que obtuvo de sus corridas, ¿Hubo una mejora de la espera constante respecto a la espera activa pura?

Espera activa con retraso pseudoaleatorio. Altere su solución al ejercicio para que en lugar de esperar exactamente la cantidad de microsegundos indicada por el usuario, espere por una cantidad pseudoaleatoria de microsegundos cuyo límite es el número indicado por el usuario en el segundo argumento de línea de comandos. Sugerencia: puede usar compilación condicional para implementar esta variación. La espera varía en cada iteración del ciclo de espera activa, algo como:

`// Random delayed busy waiting: wait until it is my turn`<br>
`while (next_thread < my_thread_id) {
	const unsigned my_delay = rand_r(&my_seed) % max_delay;
  usleep(my_delay);
}`

Ejecute al menos tres veces su solución pseudoaleatoria con la cantidad máxima de hilos y un máximo de espera de 50µs. Tome la mayor de las duraciones. ¿Hubo una mejora de la espera pseudoaleatoria respecto a la espera constante?

Comparación de las esperas. ¿Mejora el tiempo de ejecución de los dos tipos de esperas (constante y pseudoaleatoria) si disminuye o incrementa la espera máxima de los hilos? Haga al menos un par de ejecuciones con al menos los siguientes tiempos de espera:

    1µs

    5µs

    25µs

    50µs

    75µs

    100µs

Sugerencia: si hay varios compañeros(as) trabajando el mismo ejercicio en el laboratorio, escojan tiempos diferentes y compartan los resultados. Pueden crear una gráfica en un documento compartido. Agreguen más tiempos de ejecución si hay más de 6 integrantes.

Cree una gráfica donde el eje-x son las duraciones dadas por argumento al programa. El eje-y son los tiempos de ejecución de los programas. El gráfico tendrá dos series, una para la espera constante y otra para la espera pseudoaleatoria.

Agregue la gráfica al readme.md del ejercicio y una discusión de a lo sumo dos párrafos. Indique cuál fue el mejor tiempo máximo de espera obtenido y los efectos de disminuir o incrementarlo. Conjeture y trate de explicar por qué ocurre este comportamiento. Finalmente indique si la espera activa con retraso es una solución óptima, y en caso negativo, provea una idea que pueda lograr este ideal.

# Datos
| Ejecución | Espera activa |
| --- | --- |
| 1 | 0.001184151s
| 2 | 0.001280034s
| 3 | 0.001115950s

Los datos de la tabla del delay son el tiempo mayor de tres ejecuciones.

| Delay | Pseudoaleatorio | Constante
| --- | --- | --- | 
| 1µs | 0.000982483s | 0.001005767s
| 5µs | 0.001055817s | 0.001228975s
| 25µs | 0.001308450s | 0.001211558s
| 50µs | 0.001353367s | 0.001307625s
| 75µs | 0.001384167s | 0.001371517s
| 100µs | 0.001497009s | 0.001469601s

# Gráfica
![alt text](<Gráfico comparación espera activa-1.png>)

# Observaciones
Se puede notar que no hay mucha diferencia entre las columnas, especialmente considerando que la escala es de microsegundos. El mejor tiempo máximo fue de 14 microsegundos, y los diferentes delays no parecen impactar gravemente al tiempo de ejecución.

Primero, se puede observar que la columna "hello_order_busywait" no tiene mucha diferencia con las columnas que usan delay. También se puede notar que no hay mucha diferencia entre manejar los hilos con un delay constante y un delay pseudo-aleatorio. 

Lo anterior podría significar que la espera activa no es la solución para la programación paralela. Además, soluciones como el delay tampoco son factibles. Para lograr un mejor manejo de la parelelización, es mejor buscar otro algoritmo.