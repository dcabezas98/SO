/*
  Programa que se suspende hasta que reciba la señal SIGUSR1,
  para ello crea una máscara con todas las señales menos
  SIGUSR1 y se suspende con sigsuspend hasta que reciba esa
  señal (u otra no manejable).
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

int main(){

  sigset_t mask;

  sigfillset(&mask);
  sigdelset(&mask, SIGUSR1);

  sigsuspend(&mask);  
}
