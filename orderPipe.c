/*
  Programa que simula el comportamiento del cauce ("|") entre
  órdenes de linux. Duplicando la salida estándar de la primera
  y la entrada estándar de la segunda mediante la operación
  F_DUPFD de fcntl.
*/

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char * argv[]){

  setvbuf(stdout, NULL, _IONBF,0);

  pid_t pid;

  int fd[2];

  if(argc < 4 || (strcmp(argv[2], "|"))){
    printf("Error de sintaxis\n");
    printf("Uso: ./redirect order1 \"|\" order2\n");
    exit(-1);
  }

  pipe(fd);

  if((pid = fork()) < 0){
    perror("\nError en fork");
    exit(-1);
  }

  if(pid == 0){
    close(fd[0]);
    close(1);
    if(fcntl(fd[1], F_DUPFD, 1) == -1){
      perror("\nError en fcntl");
      exit(-1);
    }

    execlp(argv[1], argv[1], NULL);
  }
  else{
    close(fd[1]);
    close(0);
    if(fcntl(fd[0], F_DUPFD, 0) == -1){
      perror("\nError en fcntl");
      exit(-1);
    }

    execlp(argv[3], argv[3], NULL);
  }
}
