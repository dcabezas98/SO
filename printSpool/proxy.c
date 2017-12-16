/*
  Programa que lee información de un cliente a través de un
  fifo que ha sido copiado en la entrada estándar y la escribe
  en un archivo temporal. Cuando detecta un final de fichero 
  (EOF), comprueba con un bloqueo que la pantalla está disponible
  para imprimir y escribe toda la información. Finalmente, elimina
  el fifo mediante unlink.
*/

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define size 1024

int main(int argc, char *argv[]){

  char nombrefifo[11];
  char buffer[size];
  int fifo, lck;

  sprintf(nombrefifo, "fifo.%d", getpid());

  struct flock cerrojo;

  cerrojo.l_type = F_WRLCK;
  cerrojo.l_whence = SEEK_SET;
  cerrojo.l_start = 0;
  cerrojo.l_len = 0;

  FILE *temp = tmpfile();

  if(temp == NULL){
    perror("\nProxy: Error en tmpfile");
    exit(-1);
  }

  int leidos;

  while( (leidos = read(0, buffer, size)) > 0)
    fwrite(&buffer, 1, leidos, temp);

  if((lck = open("bloqueo", O_RDWR)) < 0){
    perror("\nProxy: Error en la apertura de bloqueo");
    exit(-1);
  }

  rewind(temp);

  if(fcntl(lck, F_SETLKW, &cerrojo) == -1){
    perror("\nProxy: Error al bloquear bloqueo");
    exit(-1);
  }

  while((leidos = fread(&buffer, 1, size, temp)) > 0)
    write(1, buffer, leidos);

  printf("\n");

  cerrojo.l_type = F_UNLCK;

  if(fcntl(lck, F_SETLK, &cerrojo) == -1){
    perror("\nProxy: Error al desbloquear bloqueo");
    exit(-1);
  }  

  if(unlink(nombrefifo) == -1)
    perror("\nProxy: Error en unlink");

  return 0;
}
