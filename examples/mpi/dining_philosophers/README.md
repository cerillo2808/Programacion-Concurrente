# Enuciado
Corrija el siguiente pseudocódigo de concurrencia de recursos compartidos para que resuelva el problema de los filósofos comensales, y por lo tanto, logre cumplir con los tres requerimientos.

```
procedure main(argc, argv[]):
  shared chopsticks[] := create_semaphores(5, 1)

  for id := 0 to 5 do
    create_thread(philosopher, id)
  end for
end procedure

procedure philosopher(id):
  while true do
    think()
    get_left_chopstick(id)
    get_right_chopstick(id)
    eat()
    put_left_chopstick(id)
    put_right_chopstick(id)
  end while
end procedure

procedure get_left_chopstick(id):
  wait(chopsticks[(id + 1) mod 5])
end procedure

procedure get_right_chopstick(id):
  wait(chopsticks[id])
end procedure

procedure put_left_chopstick(id):
  signal(chopsticks[(id + 1) mod 5])
end procedure

procedure put_right_chopstick(id):
  signal(chopsticks[id])
end procedure
```

Considere las condiciones que son necesarias para que el bloqueo mutuo o la inanición ocurran, y cambie una de ellas a la vez para generar distintas soluciones:

1. Considere la lateralidad de los filósofos.

2. Considere la cantidad de filósofos que pueden comer concurrentemente.

3. Haga que los filósofos decidan con variables protegidas por exclusión mutua.

