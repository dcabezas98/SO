Ejercicio correspondiente al exámen práctico de la asignatura sobre llamadas al sistema, 
consta de dos programas.

### Concatenar

Programa que recibe el pathname de un directorio y opcinalmente el nombre
de un archivo de salida. Recorre el directorio y por cada archivo regular
con permisos de escritura para usuario y grupo escribe en el archivo de 
salida (o en su defecto en salida estándar) su tamaño en bytes y su número
de inodo. Además, suma el tamaño en bloques de estos archivos y los escribe
en salida estándar.

### Maestro

Programa que lee una serie de argumentos, que corresponden a pathnames de directorios.
Por cada uno de ellos, crea un proceso hijo que redirige su salida estándar
a un cauce sin nombre previamente creado por el padre y ejecuta el programa 
concatenar con el directorio correspondiente y *\<PIDhijo\>Salida* como nombre
para el archivo de salida. El padre lee del cauce las sumas de bloques de los
archivos de cada directorio que imprimen los distintos programas **concatenar** y 
lo acumula en una variable para imprimirlo finalmente por pantalla.
