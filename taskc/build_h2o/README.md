# Enunciado

Hay dos tipos de hilos: los de hidrógeno y los de oxígeno. Su objetivo es formar agua con ellos. Usted dispone de un dispositivo, que cuando se coloca en él dos átomos de hidrógeno y uno de oxígeno, y se invoca bond() en cada uno de ellos, se forma agua. El dispositivo no funciona en cualquier otra combinación de átomos.

El dispositivo sólo puede trabajar en una molécula a la vez. Es decir, usted debe garantizar que los tres átomos de una molécula invoquen bond() antes de los hilos de la siguiente molécula. Escriba el código para cada átomo que garantice este comportamiento. Puede tomar el siguiente pseudocódigo como punto de partida.

```
procedure main()
  while read atom do
    case atom of
      'H': create_thread(hydrogen)
      'O': create_thread(oxygen)
    end case
  end while
end procedure

procedure hydrogen()
  bond()
end procedure

procedure oxygen()
  bond()
end procedure
```