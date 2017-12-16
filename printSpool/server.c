/*
  Programa servidor:

  Crea los fifos conocidos y el archivo de bloqueo.
  Cada vez que un cliente escribe su PID en el fifo
  de entrada para solicitar imprimir por pantalla,
  crea un hijo que:
  
  - Escribe en el fifo de salida su PID para que lo
    lea el cliente.

  - Crea un fifo cuyo nombre es su PID para que el
    cliente se comunique con el programa proxy.

  - Redirige su entrada estandar al fifo.

  - Lanza el programa proxy usando la orden exec. 

  El servidor maneja la señal SIGINT para eliminar los
  fifos de entrada y salida y el archivo de bloqueo antes
  de finalizar. Y la señal SIGCHLD para liberar los recursos
  de los hijos terminados.
*/

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>

char fifoe[64];
char fifos[64];

int w;

static void hndlr(int signum){

  if(signum == SIGCHLD){

    do{
      w = waitpid(-1, NULL, WNOHANG);
    }while(w != 0 && w != -1);
  }

  if(signum == SIGINT){
    unlink(fifoe);
    unlink(fifos);
    unlink("bloqueo");
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char *argv[]){

  if(argc < 2){
    printf("\nDebe introducirse el nombre de los fifos conocidos\n");
    exit(-1);
  }

  sprintf(fifoe, "%se", argv[1]);
  sprintf(fifos, "%ss", argv[1]);

  char nombrefifo[11];

  int fe, fs, fifo_proxy;

  int pid_proxy;
  int pid_client;

  umask(0);

  mkfifo(fifoe, 0777);
  mkfifo(fifos, 0777);

  if((fe = open(fifoe, O_CREAT|O_TRUNC|O_RDWR, 0777)) < 0){
    perror("\nServidor: Error en open del FIFO de entrada");
    exit(-1);
  }

  if((fs = open(fifos, O_CREAT|O_TRUNC|O_RDWR, 0777)) < 0){
    perror("\nServidor: Error en open del FIFO de salida");
    exit(-1);
  }

  creat("bloqueo", 0777);

  signal(SIGINT, hndlr);
  signal(SIGCHLD, hndlr);

  while(1){

    if(read(fe, &pid_client, sizeof(int)) > 0){

      if((pid_proxy = fork()) < 0){
        perror("\nServidor: Error en fork");
        exit(-1);
      }

      if(!pid_proxy){

        pid_proxy = getpid();

        sprintf(nombrefifo,"fifo.%d", pid_proxy);

        umask(0);
        if(mkfifo(nombrefifo, 0777) == -1){
          perror("\nServidor: Error en mkfifo");
          exit(-1);
        }

        if(write(fs, &pid_proxy, sizeof(int)) < 0){
          perror("\nServidor: Error al escribir el proxy");
          exit(-1);
        }

        if((fifo_proxy = open(nombrefifo, O_RDONLY)) < 0){
          perror("\nServidor: Error al abrir el fifo");
          exit(-1);
        }

        if(dup2(fifo_proxy, 0) == -1){
          perror("\nServidor: Error en dup2");
          exit(-1);
        }
        execlp("./proxy", "proxy", NULL);
      }
    }
  }
}
