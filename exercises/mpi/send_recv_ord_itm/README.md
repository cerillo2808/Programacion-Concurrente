# Enunciado

Modifique la solución a la [send_recv_ord_sem](../send_recv_ord_sem/) para que los procesos saluden en orden por su identificador (rank) a través de un intermediario. Es decir, los procesos envían su saludo a uno del equipo, quien recibe los saludos y los imprime en la salida estándar en orden.

Al usar un itermediario con comunicación punto a punto, se simula "seguridad condicional". Note que los saludos son información vectorial y no escalar. Tenga presente que no es posible enviar datos discontinuos de la memoria a través de la red en una operación de send/receive.

# Caso de prueba
Para 3 procesos:
![alt text](image.png)