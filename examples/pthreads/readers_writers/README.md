# Enunciado

Permita que varios lectores puedan leer de un medio común, pero sólo un escritor puede modificarlo a la vez. Por cada letra R que se lea de la entrada estándar se crea un hilo lector (reader), y por cada W que se lea de la entrada estándar se crea un hilo escritor (writer). Varios hilos pueden invocar la función read() al mismo tiempo, pero sólo un único hilo puede invocar write() a la vez. Esta puede considerarse una exlusión mutua categórica (categorical mutual exclusion) porque permite paralelismo de una categoría de hilos, pero excluye los de otra.

```
procedure main()
  while true do
    case read_char() of
      'R': create_thread(reader)
      'W': create_thread(writer)
      EOF: return
    end case
  end while
end procedure

procedure reader()
  read()
end procedure

procedure writer()
  write()
end procedure
```