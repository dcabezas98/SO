// David Cabezas Berrido

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[]){

  int pid, fd[2];

  unsigned long suma = 0;
  unsigned long bloques;

  pipe(fd);

  int i;

  for(i = 1; i < argc; i++){

    if((pid = fork()) < 0){ 
      perror("\nError en fork");
      exit(-1);
    }

    if(!pid){  // Hijo

      close(fd[0]);  // Cierra lectura

      char salida[12];
      sprintf(salida, "%dSalida", getpid());

      dup2(fd[1], STDOUT_FILENO);  // Copia salida estándar 
      execlp("./concatenar", "concatenar", argv[i], salida, NULL);
    }
    else{  // Padre
      wait(NULL);  // Espera a que el hijo escriba
      read(fd[0], &bloques, sizeof(unsigned long));  // Lee lo que ha escrito
      suma += bloques;  // Lo acumula
    }	
  }

  printf("Suma bloques: %lu\n", suma);
  exit(0);
}
