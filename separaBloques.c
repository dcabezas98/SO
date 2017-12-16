/*
  Programa que lee el nombre de un programa como argumento y 
  separa su contenido en bloques de 80 bytes.

  Escribe en un fichero de salida llamado salida.txt el número
  de bloques y el contenido de cada bloque.
*/

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>  	
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

int main(int argc, char *argv[])
{

  int ifd, ofd;

  int n = 0;

  char caracter[1];
  char bloque[256];
  int numChar = 0;
  int nbytes;

  if( (ifd=open(argv[1],O_RDONLY))<0) {
    printf("\nError %d en open",errno);
    perror("\nError en open");
    exit(EXIT_FAILURE);
  }

  umask(0);
  
  if( (ofd=open("salida.txt",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
    printf("\nError %d en open de la salida",errno);
    perror("\nError en open de la salida");
    exit(EXIT_FAILURE);
  }

  nbytes = lseek(ifd, 0, SEEK_END);
  lseek(ifd, 0, SEEK_SET);

  sprintf(bloque, "El número de bloques es %d", (nbytes+80)/80);
  write(ofd,bloque,strlen(bloque));
  
  while(read(ifd,caracter,1) > 0) {
    if(numChar%80 == 0){
      n++;
      sprintf(bloque,"\n\nBloque %d\n",n);
      write(ofd,bloque,strlen(bloque));
    }
    write(ofd,caracter,1);
    numChar++;
  }

  close(ifd);
  close(ofd);
  
  return EXIT_SUCCESS;
}
