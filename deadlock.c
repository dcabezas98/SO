/*
  Programa que fuerza una situación de interbloqueo para
  asegurarse de que el sistema operativo es capaz de evitarlo.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main (int argc, char * argv[]){

  struct flock cerrojo1, cerrojo2;
  int fd;

  pid_t pid;

  if((fd = open(*(argv+1), O_RDWR)) == -1){
    perror("\nError en open");
    exit(-1);
  }

  cerrojo1.l_type = F_WRLCK;
  cerrojo1.l_whence = SEEK_SET;
  cerrojo1.l_start = 0;
  cerrojo1.l_len = 100;

  cerrojo2 = cerrojo1;
  cerrojo2.l_start = 300;

  // Padre
  if(fcntl(fd, F_SETLKW, &cerrojo1) == -1){
    perror("\nPadre: error con cerrojo1");
    exit(-1);
  }

  printf("Padre: cerrojo1 el archivo\n");

  if((pid = fork()) < 0){
    perror("\nError en fork");
    exit(-1);
  }

  if(pid == 0){ // Hijo

    if(fcntl(fd, F_SETLKW, &cerrojo2) == -1)
      perror("\nHijo: error en cerrojo2");

    else printf("Hijo: cerrojo2 en el archivo\n");

    //sleep(1); // El hijo bloqueará antes y el sistema operativo hará que el padre falle
    sleep(3);   // El hijo bloqueará después y el sistema operativo hará que el hijo falle

    if(fcntl(fd, F_SETLKW, &cerrojo1) == -1)
      perror("\nHijo: error en cerrojo1");

    exit(0);
  }
  else{ // Padre

    sleep(2);

    if(fcntl(fd, F_SETLKW, &cerrojo2) == -1)
      perror("\nPadre: error en cerrojo2");

    else printf("Padre: cerrojo2 en el archivo\n");
  }

  waitpid(-1, NULL, WNOHANG);

  return 0;
}
