/*
  Programa que recibe los extremos de un intervalo de enteros,
  lo divide en dos mitades, y crea dos hijos que ejecutan el
  programa slave y calculan los números primos en cada uno de
  los dos intervalos.

  Crea pipes sin sin nombre y redirige las salidas estándar de los
  esclavos y su entrada estándar a los pipes mediante dup2. 
*/

#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, char *argv[]){

  if(argc != 3){
    perror("\nDeben introducirse los dos extremos de un intervalo");
    exit(EXIT_FAILURE);
  }

  pid_t pid;

  int fd1[2];  // Descriptores del cauce con el primer esclavo
  int fd2[2];  // Descriptores del cauce con el segundo esclavo

  char mitad1[1000000];
  char mitad2[1000000];

  int num;

  sprintf(mitad1,"%d", (atoi(argv[1]) + atoi(argv[2]))/2); // Divido el intervalo
  sprintf(mitad2,"%d", (atoi(argv[1]) + atoi(argv[2]))/2 +1);

  char *arg1[4];  // Argumentos para el primer esclavo
  arg1[0] = "slave";
  arg1[1] = argv[1];
  arg1[2] = mitad1;
  arg1[3] = NULL;

  char *arg2[4];  // Argumentos para el segundo esclavo
  arg2[0] = "slave";
  arg2[1] = mitad2;
  arg2[2] = argv[2];
  arg1[3] = NULL;

  pipe(fd1);

  if( (pid = fork()) < 0) {
    perror("\nError en fork 1");
    exit(EXIT_FAILURE);
  }

  if(pid == 0) {

    close(fd1[0]);
    dup2(fd1[1], STDOUT_FILENO);

    if( execvp("./slave", arg1) < 0){
      perror("\nError en exec 1");
      exit(EXIT_FAILURE);
    }
  }
  else {

    close(fd1[1]);
    dup2(fd1[0], STDIN_FILENO);

    while(read(fd1[0], &num, sizeof(int)) > 0)
      printf("%d\n", num);

    close(fd1[0]);
  }

  pipe(fd2);

  if( (pid = fork()) < 0){
    perror("\nError en fork 2");
    exit(EXIT_FAILURE);
  }

  if(pid == 0) {

    close(fd2[0]);
    dup2(fd2[1], 1);

    if( execvp("./slave", arg2) < 0){
      perror("\nError en exec 2");
      exit(EXIT_FAILURE);
    }
  }
  else {

    close(fd2[1]);
    dup2(fd2[0], 0);

    while(read(fd2[0], &num, sizeof(int)) > 0)
      printf("%d\n", num);

    close(fd2[0]);
  }

  return EXIT_SUCCESS;
  }
