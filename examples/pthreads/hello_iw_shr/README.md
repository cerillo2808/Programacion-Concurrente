# Enunciado
Cree una nueva versión de su programa "hola mundo numerado" donde cada hilo de ejecución imprime "Hello from secondary thread i of w", donde i es el número de hilo y w la cantidad total de hilos que componen el equipo, pero con los siguientes cambios:

1. Haga que los datos comunes para todos los hilos de ejecución (el número w) estén en un registro (estructura) compartido para cada hilo. Mantenga los datos exclusivos para cada hilo (i) se mantengan en el registro privado.

2. Reporte la duración en segundos que los hilos toman en saludar. Puede usar la función POSIX clock_gettime() de <time.h>.

# Observaciones
- En vez de usar los datos como variables, se usan como un campo de registro compartido.
- Es muy marcada la diferencia entre memoria privada y compartida. Un hilo puede tener ambas a la vez. O sea, compartir ciertas cosas, y mantener privadas otras.