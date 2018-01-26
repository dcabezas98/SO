Práctica final sobre llamadas al sistema.

##### Instrucciones de uso:

Ejecutar el programa **server** utilizando como argumento el nombre
para los fifos de entrada y salida.

A continuación, ejecutar el programa **clientes** utilizando como
argumentos ese mismo nombre y el número de clientes (entre 1 y 10).

### clientes

Programa que crea entre 1 y 10 clientes para imprimir por pantalla,
cada cliente imprime un único caracter, entre 1 y 10000 veces.

### proxy 

Programa que lee información de un cliente a través de un
fifo que ha sido copiado en la entrada estándar y la escribe
en un archivo temporal. Cuando detecta un final de fichero 
(EOF), comprueba con un bloqueo que la pantalla está disponible
para imprimir y escribe toda la información. Finalmente, elimina
el fifo mediante unlink.

### server

Programa que crea los fifos conocidos y el archivo de bloqueo.
Cada vez que un cliente escribe su PID en el fifo
de entrada para solicitar imprimir por pantalla,
crea un hijo que:
  
- Escribe en el fifo de salida su PID para que lo
    lea el cliente.

- Crea un fifo cuyo nombre es su PID para que el
    cliente se comunique con el programa proxy.

- Redirige su entrada estandar al fifo.

- Lanza el programa proxy usando la orden exec. 

El servidor maneja la señal SIGINT para eliminar los
fifos de entrada y salida y el archivo de bloqueo antes
de finalizar. Y la señal SIGCHLD para liberar los recursos
de los hijos terminados.
