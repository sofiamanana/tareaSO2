# Tarea 2 Sistemas Operativos

#### Integrantes:
- Alfredo Llanos 201804536-5
- Sofia Mañana 201804535-7

#### Indicaciones:
- Programa desarrollado en sistema operativo Ubuntu 20.04 con lenguaje de programacion C.
- Ejecutar make run en la Linea de Comando.

#### Implementacion:
- Se utilizo memoria compartida para generar el tablero, donde cada cuadrado de este esta definido por un struct que contiene un entero con el efecto de cada casilla. Tambien se utilizo para definir un arreglo de enteros, en sus primeras 4 posiciones contiene la posicion de los 4 jugadores, en la posicion 5 esta una flag para la salida para cuando un jugador gana el juego, en la posicion 6 esta una flag de cuando los turnos van en orden (0) o en sentido contrario (1) y en la posicion 7 se guarda el id del jugador que gano el juego.
- Para las pipes, se genero una pipe entre el proceso 0 y el 1, otra entre el proceso 1 y el 2, otra entre el proceso 2 y el 3 y otra entre el proceso 3 y el 0. Con esto se genera un circulo de mensajes donde se mantiene el orden de los turnos y cada uno espera al anterior para continuar.
- Para la continuacion de los turnos, una vez que cada uno haya terminado, envia un 1 al siguiente proceso, lo cual indica que le toca al siguiente proceso, si se envia un 2, el siguiente proceso lo lee y no ejerce su turno y manda un 1 al siguiente. Si se manda un 3, sigue el orden de los turnos restando 1 en cada proceso y cuando llegue a 1, se ejecutara ese proceso, con esto se logra dar vuelta los turnos sin alterar el orden de los pipes.
- Una vez que se cumpla la igualdad del while, se sale de este y se imprime el id del jugador ganador.

