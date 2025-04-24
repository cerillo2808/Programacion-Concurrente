# Enunciado

Modifique su solución al [network_simul_packet_loss](../network_simul_packet_loss) para que el hilo que extravía paquetes esté ubicado entre el productor y el repartidor, de tal forma, que el porcentaje de pérdida de paquetes afecte a todos los paquetes y no una fracción de los mensajes de red.

¿Cuántas líneas de código debió modificar para reflejar este cambio? <br>
¿Cuántas de esas líneas fueron en la implementación de su <br>(a) productor, <br>
(b) repartidor, <br>
(c) consumidor, <br>
(d) ensamblador, y <br>
(e) controlador de la simulación?

# Comentarios

En total, modifiqué 6 líneas.
- Repartidor: 1
- Consumidor: 1
- Ensamblador: 2
- Controlador: 1

La modularización de los archivos es muy importante para poder hacer cambios relevantes sin tener que modificar mucho código.