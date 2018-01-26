### slave

Programa que recibe los extremos de un intervalo de enteros
e imprime por la salida estandar los números primos en ese
intervalo.

### master

Programa que recibe los extremos de un intervalo de enteros,
lo divide en dos mitades, y crea dos hijos que ejecutan el
programa slave y calculan los números primos en cada uno de
los dos intervalos.

Crea pipes sin sin nombre y redirige las salidas estándar de los
esclavos y su entrada estándar a los pipes mediante dup2. 
