
/*
  Programa que recibe los extremos de un intervalo de enteros
  e imprime por la salida estandar los números primos en ese
  intervalo.

  Puede ser llamado por un master y calcularlos para él.
*/

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int main(int argc, char *argv[]){

  if(argc != 3)
    exit(EXIT_FAILURE);

  int primo, i, j;

  for(i = atoi(argv[1]); i <= atoi(argv[2]); i++){
    for(j = 2, primo = 1; j <= sqrt(i) && primo; j++){
      if(i % j == 0) primo = 0;
    }

    if(primo){
      write(1, &i, sizeof(int));
    }
  }

  return EXIT_SUCCESS;
}
