/*
  Programa que redirecciona la entrada y salida de órdenes
  de linux, duplicando en el descriptor del archivo dado 
  como argumento la entrada o salida estándar, según corresponda.
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

  if(argc < 4 || (strcmp(argv[2], "<") && strcmp(argv[2], ">"))){
    printf("Error de sintaxis\n");
    printf("Uso: ./redirect orden \"</>\" archivo\n");
    exit(-1);
  }

  int fd = open(argv[3], O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

  if(strcmp(argv[2], "<") == 0){
    close(0);
    if(fcntl(fd, F_DUPFD, 0) == -1){
      perror("\nError en fcntl");
      exit(-1);
    }
  }
  else{
    close(1);
    if(fcntl(fd, F_DUPFD, 1) == -1){
      perror("\nError en fcntl");
      exit(-1);
    }      
  }

  execlp(argv[1], argv[1], NULL);
}
