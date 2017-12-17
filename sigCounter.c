/*
  Programa que implementa un manejador de señales que cuenta
  el número de veces que se ha recibido la señal.
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

static int count[31];

static void counderHndlr(int sigNum){

  count[sigNum]++;

  printf("Número de llamadas a la señal %d: %d\n", sigNum, count[sigNum]);
  
}

int main() {

  struct sigaction saCounter;

  setvbuf(stdout, NULL, _IONBF, 0);

  saCounter.sa_handler = counderHndlr;

  sigemptyset(&saCounter.sa_mask);
  saCounter.sa_flags = SA_RESTART;

  for(int i = 0; i < 31; i++)
    count[i] = 0;

  printf("No puedo manejar la señal %d\n", SIGKILL);
  printf("No puedo manejar la señal %d\n", SIGSTOP);

  for(int i = 0; i < 31; i++){
    sigaction(i, &saCounter, NULL);
  }

  for(;;){}
}
