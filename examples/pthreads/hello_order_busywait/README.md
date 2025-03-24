# Enunciado
Haga que los threads saluden siempre en orden. Es decir, si se crean w threads, la salida sea siempre en orden

`Hello from thread 0 of w` <br>
`Hello from thread 1 of w` <br>
`Hello from thread 2 of w` <br>
`...` <br>
`Hello from thread w of w` <br>

Utilice espera activa como mecanismo de sincronización (control de concurrencia).

Ejecute varias veces su solución de hello_order_busywait con la cantidad máxima de threads que el su sistema operativo le permite. ¿Es consistente la duración de su solución con espera activa?

Describa el comportamiento del programa y explique la razón de este comportamiento en el archivo readme.md dentro de su carpeta hello_order_busywait. Indique en qué condiciones sería apremiante utilizar espera activa, o en caso de no haberlas, sugiera soluciones alternativas.

# Observaciones
El programa siempre imprime en orden, lo cual podría ser útil para algunos casos. La razón por la que los hilos se ejecutan en orden es por el busywait, o la espera activa. Esta es una mala práctica de programación porque entre más hilos se crean, más se nota el efecto de la paralización de la máquina. 

- El hilo principal sufre de indeterminismo. Los hilos secundarios se imprimen en orden, pero como el hilo principal no fue programada con la espera activa, en la ejecución el print del main se mueve.