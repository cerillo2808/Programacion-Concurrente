# Patrones básicos de concurrencia de tareas
## Tabla de contenidos
- [Patrones básicos de concurrencia de tareas](#patrones-básicos-de-concurrencia-de-tareas)
  - [Tabla de contenidos](#tabla-de-contenidos)
  - [1. Rutas de ejecución](#1-rutas-de-ejecución)
    - [Respuesta](#respuesta)
  - [2. Rutas de ejecución extremas](#2-rutas-de-ejecución-extremas)
    - [Respuestas](#respuestas)
  - [3. Signaling](#3-signaling)
    - [3.1. 2 hilos](#31-2-hilos)
      - [Respuesta](#respuesta-1)
    - [3.2. 3 hilos](#32-3-hilos)
      - [Respuesta](#respuesta-2)
    - [3.3. w hilos](#33-w-hilos)
  - [4. Encuentro (rendezvous)](#4-encuentro-rendezvous)
    - [4.1. Ejercicio: Encuentro de ajedrez](#41-ejercicio-encuentro-de-ajedrez)
  - [5. Exclusión mutua con semáforos (mutex)](#5-exclusión-mutua-con-semáforos-mutex)
    - [5.1 Asimétrica \[sem\_mutex\_asym\]](#51-asimétrica-sem_mutex_asym)
    - [5.2 Simétrica \[sem\_mutex\_sym\]](#52-simétrica-sem_mutex_sym)
  - [6. Exclusión mutua acotada (multiplex)](#6-exclusión-mutua-acotada-multiplex)
  - [7. Barrera con semáforos (barrier)](#7-barrera-con-semáforos-barrier)
    - [7.1. Barrera de una pasada \[sem\_barrier\_1pass\]](#71-barrera-de-una-pasada-sem_barrier_1pass)
    - [7.2. Barrera reusable con semáforos \[sem\_barrier\_reusable\]](#72-barrera-reusable-con-semáforos-sem_barrier_reusable)

## 1. Rutas de ejecución
Dos threads ejecutan las siguientes tareas. Liste todas las rutas de ejecución indicando el valor final de la variable x y la salida estándar. Una ruta de ejecución (execution path) es una combinación de instrucciones que depende del orden en que sean ejecutadas.

````
procedure main()
  shared x := 0
  create_thread(thread_a)
  create_thread(thread_b)
end procedure

procedure thread_a()
  x := 5
  print(x)
end procedure

procedure thread_b()
  x := 7
end procedure
````

### Respuesta
Ruta 1 <br>
Salida estándar: 5 <br>
Valor final: 7
````
x :=5
print(x)
x :=7
````

Ruta 2 <br>
Salida estándar: 7 <br>
Valor final: 7
````
x :=5
x :=7
print(x)
````

Ruta 3 <br>
Salida estándar: 5 <br>
Valor final: 5
````
x :=7
x :=5
print(x)
````

## 2. Rutas de ejecución extremas
Suponga que el hilo de ejecución principal crea w=100 hilos secundarios con los siguientes códigos.

````
procedure main()
  shared count := 0
  create_threads(100, secondary)
end procedure

procedure secondary()
  for i := 0 to 100 do
    const temp := count
    count := temp + 1
  end for
end procedure
````
### Respuestas

1. ¿Cuál es el valor más grande que la variable compartida count podría llegar a obtener? ¿En qué rutas de ejecución se alcanza este valor?

R/ Cada hilo hace 101 incrementos, y hay 100 hilos. En total, la variable compartida podría llegar a 101*100=10100. Se alcanza este valor cuando los hilos se ejecutan en orden, o bien, cuando no hay condiciones de carrera.

2. ¿Cuál es el menor valor que la variable compartida count podría llegar a obtener? ¿En qué rutas de ejecución se alcanza este valor?

R/ El menor valor es 101 y sucedería cuando todos los hilos se ejecutan a la vez y sobreescriben el mismo valor.

## 3. Signaling
Haga que la instrucción `a1` se ejecute siempre antes que la instrucción `b1`. Esto es, que un hilo envíe una señal (aviso) a otro (signaling).

### 3.1. 2 hilos
Haga que la instrucción a1 se ejecute siempre antes que la instrucción b1. Esto es, que un hilo envíe una señal (aviso) a otro (signaling).

````
procedure main()
  create_thread(thread_a)
  create_thread(thread_b)
end procedure

procedure thread_a()
  statement a1
end procedure

procedure thread_b()
  statement b1
end procedure
````

#### Respuesta
````
procedure main()
    shared a1_ready := create_semaphore(0) // can_run_b1
    create_thread(thread_a)
    create_thread(thread_b)
end procedure

procedure thread_a()
    statement a1
    signal(a1_ready)
end procedure

procedure thread_b()
    wait(a1_ready)
    statement b1
end procedure
````

### 3.2. 3 hilos
Haga que la instrucción `a1` se ejecute siempre antes que la instrucción `b1` y ésta siempre se ejecute antes que la instrucción `c1`. Este orden de ejecución puede abreviarse como `a1 < b1 < c1`.

````
procedure main()
  create_thread(thread_a)
  create_thread(thread_b)
  create_thread(thread_c)
end procedure

procedure thread_a()
  statement a1
end procedure

procedure thread_b()
  statement b1
end procedure

procedure thread_c()
  statement c1
end procedure
````

#### Respuesta
Resuelto la clase pasada en: [signaling_3](/exercises/pthreads/signaling_3)

### 3.3. w hilos
Generalice el patrón de aviso (signaling) para dada una cantidad arbitraria `w` de hilos que ejecutan la instrucción `a`, lo hagan en el orden del número de hilo. Por ejemplo, si `ai` es la ejecución de la instrucción `statement a` por parte del hilo con número `i`, entonces se asegure que siempre se ejecute la secuencia `a0 < a1 < a2 < a3 < …​ < aw`.

````
procedure main()
  input shared thread_count
  create_threads(thread_count, secondary)
end procedure

procedure secondary(thread_number)
  statement a
end procedure
````

Una solución a este problema se había hecho antes en el curso ¿recuerda en cuál ejemplo?

R/ [hello_order_semaphor](/examples/pthreads/hello_order_semaphor/), donde se usa un arreglo de semáforos.
````
procedure main()
  shared semaphores[0..w-1] := [1, 0, 0, ..., 0] // el semáforo 0 inicia desbloqueado
  input shared thread_count
  create_threads(thread_count, secondary)
end procedure

procedure secondary(thread_number)
  wait(semaphores[thread_number])
  statement a
  if thread_number + 1 < thread_count
    signal(semaphores[thread_number + 1])
end procedure
````

## 4. Encuentro (rendezvous)
Haga que la instrucción `a1` y `b1` se ejecuten siempre antes que las instrucciones `a2` y `b2`. Este problema tiene el nombre francés rendezvous  que significa encuentro en un punto de ejecución, y ninguno de los dos hilos pueden continuar su ejecución hasta que el otro haya llegado. Nota: Puede crear varias soluciones a este problema.

````
procedure main()
  create_thread(thread_a)
  create_thread(thread_b)
end procedure

procedure thread_a()
  statement a1
  statement a2
end procedure

procedure thread_b()
  statement b1
  statement b2
end procedure
````

#### Respuesta
````
procedure main()
  create_thread(thread_a)
  create_thread(thread_b)
  shared sem_a := create_semaphore(0)
  shared sem_b := create_semaphore(0)
end procedure

procedure thread_a()
  statement a1
  signal(sem_a)       // a1_done
  wait(sem_b)         // wait_b1
  statement a2
end procedure

procedure thread_b()
  statement b1
  signal(sem_b)       // b1_done
  wait(sem_a)         // wait_a1
  statement b2
end procedure
````

### 4.1. Ejercicio: Encuentro de ajedrez
En un torneo de ajedrez los participantes efectúan el siguiente itinerario tradicional. Los jugadores y árbitros se identifican para ingresar al centro deportivo (`enter_room`). Cada jugador ubica la mesa con su tablero y se sienta en ella. Una vez que ambos jugadores están presentes, avisan al árbitro. El árbitro establece el tiempo en el reloj (`set_clock`) del tablero y con ello ambos jugadores inician la partida (`play_chess`).

El problema se considera resuelto si el árbitro establece el reloj sólo hasta que los dos jugadores hayan ingresado al centro deportivo (`enter_room`) y los jugadores juegan (`play_chess`) hasta que el árbitro haya establecido el reloj (`set_clock`). Considere el siguiente código inicial que trata de representar el escenario descrito.

Agregue control de concurrencia al código inicial para considerando todas las posibles rutas de ejecución, el problema siempre esté resuelto.
````
procedure main()
  shared player1_ready := semaphore(0)
  shared player2_ready := semaphore(0)
  shared players_ready := semaphore(0)
  shared clock_ready := semaphore(0)

  create_thread(player1)
  create_thread(player2)
  create_thread(referee)
end procedure

procedure player1()
  enter_room()
  play_chess()
end procedure

procedure player2()
  enter_room()
  play_chess()
end procedure

procedure referee()
  enter_room()
  set_clock()
end procedure
````

#### Respuesta
````
procedure main()
  shared players_ready_count := 0
  shared mutex := semaphore(1)
  shared players_ready := semaphore(0)     // usado por referee
  shared clock_ready := semaphore(0)       // usado por jugadores

  create_thread(player1)
  create_thread(player2)
  create_thread(referee)
end procedure

procedure player1()
  enter_room()

  wait(mutex)
  players_ready_count := players_ready_count + 1
  if players_ready_count = 2 then
    signal(players_ready) // avisa al referee que ambos jugadores han llegado
  end if
  signal(mutex)

  wait(clock_ready) // espera a que el árbitro haya puesto el reloj
  play_chess()
end procedure

procedure player2()
  enter_room()

  wait(mutex)
  players_ready_count := players_ready_count + 1
  if players_ready_count = 2 then
    signal(players_ready)
  end if
  signal(mutex)

  wait(clock_ready)
  play_chess()
end procedure

procedure referee()
  enter_room()
  wait(players_ready)  // espera hasta que ambos jugadores estén listos
  set_clock()
  signal(clock_ready)
  signal(clock_ready)  // señal para ambos jugadores
end procedure
````

## 5. Exclusión mutua con semáforos (mutex)
### 5.1 Asimétrica [sem_mutex_asym]
Agregue semáforos al pseudocódigo siguiente para forzar a que los incrementos en los hilos se hagan con exclusión mutua.

````
procedure main()
  shared count := 0
  create_thread(thread_a)
  create_thread(thread_b)
end procedure

procedure thread_a()
  count := count + 1
end procedure

procedure thread_b()
  count := count + 1
end procedure
````

> Un semáforo inicializado en 1 y que nunca supera este valor, es un semáforo binario. Es equivalente a un mutex excepto que no tiene dueño (ownership). Cuando el semáforo tiene el valor 1 indica que el mutex está disponible, y el valor 0 que está bloqueado. Se diferencia en que un mutex nunca supera el valor 1, mientras que un semáforo técnicamente puede hacerlo, y de que un mutex sólo puede ser incrementado por el mismo thread que lo decrementó.

#### Respuesta
````
procedure main()
  shared count := 0
  shared mutex := semaphore(1)  // semáforo binario para exclusión mutua

  create_thread(thread_a)
  create_thread(thread_b)
end procedure

procedure thread_a()
  wait(mutex)
  count := count + 1
  signal(mutex)
end procedure

procedure thread_b()
  wait(mutex)
  count := count + 1
  signal(mutex)
end procedure
````

### 5.2 Simétrica [sem_mutex_sym]
Note que en la actividad anterior ambos threads ejecutaban el código en subrutinas distintas, a lo que se le llama una solución asimétrica (separación de asuntos o concurrencia de tareas). Sin embargo, el código de las subrutinas era el mismo y por tanto podría convertirse en una solución simétrica. En una solución simétrica los hilos ejecutan el mismo código, es decir, la misma subrutina.

Las soluciones simétricas son a menudo más fáciles de generalizar. Agregue semáforos al pseudocódigo siguiente para forzar a que los incrementos hechos por una cantidad arbitraria de hilos sean con exclusión mutua.

````
procedure main()
  shared count := 0
  input const thread_count
  create_threads(thread_count, secondary)
end procedure

procedure secondary()
  // Critical section
  count := count + 1
end procedure
````

#### Respuesta
````
procedure main()
  shared count := 0
  shared mutex := semaphore(1)  // Semáforo binario
  input const thread_count

  create_threads(thread_count, secondary)
end procedure

procedure secondary()
  wait(mutex)                  // Entra a la sección crítica
  count := count + 1
  signal(mutex)                // Sale de la sección crítica
end procedure
````

## 6. Exclusión mutua acotada (multiplex)
Modifique el pseudocódigo dado para que imponga una cota o límite superior de n hilos ejecutando una sección de concurrencia acotada. A este patrón se le llama multiplex y es útil para problemas donde se tienen distintos entes trabajando al mismo tiempo pero con máximo de capacidad.

Por ejemplo, la cantidad de cajeros atendiendo en las ventanillas de un banco o de clientes patinando en la pista del salón de patines. En este último caso, si la capacidad de la pista se agota, algunos clientes tendrán que esperar afuera, y apenas una persona sale de la pista, otra podrá ingresar de inmediato. Use el código siguiente para reflejar este comportamiento.

````
procedure main()
  input const skater_count, const room_capacity
  create_threads(skater_count, skater)
end procedure

procedure skater()
  // Concurrency-bounded region
  skate()
end procedure
````

## 7. Barrera con semáforos (barrier)
### 7.1. Barrera de una pasada [sem_barrier_1pass]
Generalice su solución a la actividad rendezvous ([act_rendezvous]) para asegurar que una cantidad arbitraria de hilos no continúen su ejecución hasta que todos hayan alcanzado (ejecutado) el punto de encuentro. Es decir, si se crean `w` hilos, los primeros `w - 1` hilos que lleguen al punto de encuentro deberían esperar hasta que el hilo en la posición `w` llegue al punto de encuentro. En tal momento todos los w threads podrán continuar ejecutándose concurrentemente. A este patrón se le conoce como barrera (barrier). Implemente una barrera con semáforos en el siguiente pseudocódigo para que la instrucción `Statement B` se ejecute sólo hasta que todos los hilos hayan ejecutado la instrucción `Statement A`.
````
procedure main()
  // How many threads have arrived to the barrier
  shared count := 0
  // Protects the increment of the count
  shared can_access_count := create_semaphore(1)
  // Locked (0) until all threads arrive, then it is unlocked (1)
  shared barrier := create_semaphore(0)
  // Read thread count from standard input
  input shared const thread_count
  // Create a thread team running secondary
  create_threads(thread_count, secondary)
end procedure

procedure secondary()
  Statement A
  // Adapt rendezvous solution here
  // Statement B can be only executed until all threads have run Statement A
  Statement B
end procedure
````

#### Respuesta
````
procedure main()
  shared count := 0
  shared can_access_count := semaphore(1)
  shared barrier := semaphore(0)
  input shared const thread_count
  create_threads(thread_count, secondary)
end procedure

procedure secondary()
  Statement A
  wait(can_access_count)
  count := count + 1
  if count = thread_count then
    for i := 1 to thread_count do
      signal(barrier)
  signal(can_access_count)
  wait(barrier)
  Statement B
end procedure
````

### 7.2. Barrera reusable con semáforos [sem_barrier_reusable]
Haga que su solución a la actividad [sem_barrier] sea reusable. Es decir, que la barrera quede lista para volver a usarse, por ejemplo, dentro de un ciclo. Debe asegurarse de que el contador quede en 0, y los threads que salen de la barrera no se combinen con otros que aún están en ella.

Sugerencia: Puede reemplazar la barrera por dos torniquetes. Un torniquete (turnstile) o "trompo" es un dispositivo de control de acceso que impide o da paso a una secuencia de personas de acuerdo a ciertas condiciones. Se usa típicamente en instalaciones deportivas, transporte público, acceso a edificios, parques de diversiones, y otros.

Un torniquete es un semáforo que detiene a todos los hilos que lleguen a él. Pero apenas un hilo logre pasarlo, inmediatamente habilita el paso al siguiente. Puede pensarse como una manera más elegante de que el último hilo que llega a la barrera haga un ciclo de incrementos por todos los demás.

````
procedure main()
  ...
  // Stops all threads, but the first one that passess it passess the rest
  shared turnstile := create_semaphore(0)
  ...
end procedure

procedure secondary()
  while true do
    Statement A
    ...
    // Use turnstile
    wait(turnstile)
    signal(turnstile)
    ...
    // Statement B can be only executed until all threads have run Statement A
    Statement B
  end while
end procedure
````

#### Respuesta
````
procedure main()
  shared count := 0
  shared mutex := semaphore(1)
  shared turnstile1 := semaphore(0)
  shared turnstile2 := semaphore(1)  // cerrado inicialmente
  input shared const thread_count

  create_threads(thread_count, secondary)
end procedure

procedure secondary()
  while true do
    Statement A
    wait(mutex)
    count := count + 1
    if count == thread_count then
      wait(turnstile2)            // cerrar turnstile2
      signal(turnstile1)          // abrir turnstile1
    signal(mutex)
    wait(turnstile1)              // pasar torniquete 1
    signal(turnstile1)            // permitir al siguiente pasar
    Statement B
    wait(mutex)
    count := count - 1
    if count == 0 then
      wait(turnstile1)            // cerrar turnstile1
      signal(turnstile2)          // abrir turnstile2
    signal(mutex)
    wait(turnstile2)              // pasar torniquete 2
    signal(turnstile2)            // permitir al siguiente salir
end procedure

````