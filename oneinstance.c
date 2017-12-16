/*
  Programa que se asegura de que sólo exista una instancia de él
  en ejecución al mismo tiempo, esto lo hace bloqueando un archivo
  que crea en la carpeta /tmp. El programa ejecuta un sleep(10) y 
  finaliza, y durante ese tiempo no se pueden lanzar otras instancias
  del mismo programa.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(){

  char blockfile[] = "/tmp/BlockFile";
  struct flock cerrojo;
  int fd;

  umask(0);

  if((fd = open(blockfile, O_CREAT|O_WRONLY, S_IWUSR)) < 0){
    perror("\nError en open");
    exit(-1);
  }

  cerrojo.l_type = F_WRLCK;
  cerrojo.l_whence = SEEK_SET;
  cerrojo.l_start = 0;
  cerrojo.l_len = 0;

  if(fcntl(fd, F_SETLK, &cerrojo) == -1){
    if(errno & (EACCES | EAGAIN)){
      printf("Ya se está ejecutando otra instancia de este programa\n");
      exit(-1);
    }
  }

  printf("Soy la única instancia del programa\n");

  sleep(10);

  return 0;
}
