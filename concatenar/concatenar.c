// David Cabezas Berrido

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

int main(int argc, char *argv[]){

  if(argc < 2){
    perror("\nDebe introducirse un directorio argumento");
    exit(-1);
  }

  DIR *dir;
  struct dirent *arch;
  struct stat st;
  int fd,PID, fsalida;

  umask(0);
  if(argc >= 3){  
    if((fsalida = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0700)) < 0){  // Creo salida
      perror("\nError apertura fichero de salida");
      exit(-1);
    }
  }
  else fsalida = STDOUT_FILENO;  // Si no se recibe se usa la estándar

  dir=opendir(argv[1]);
  arch = readdir(dir);

  unsigned long suma = 0;

  char path[64];
  char num[32];

  while(arch != NULL){  //  Recorro el directorio
		
    sprintf(path,"%s/%s",argv[1],arch->d_name);
    stat(path, &st);
		
    if(S_ISREG(st.st_mode) && (st.st_mode & S_IWUSR) && (st.st_mode & S_IWGRP)){  // Compruebo que el archivo sea del tipo que piden
			
      if(write(fsalida, path, strlen(path)+1) < 0){  // Nombre en el fichero de salida
	perror("\nError de escritura");
	exit(-1);
      }
			
      sprintf(num, "%d", st.st_size); 

      write(fsalida, "  ", 3);

      write(fsalida, num, strlen(num)+1);  //  Size (bytes)

      write(fsalida, "  ", 3);

      sprintf(num, "%d", st.st_ino);
			
      write(fsalida, num, strlen(num)+1);  // Numero inodo

      write(fsalida, "\n", strlen("\n")+1);

      suma += st.st_blocks;  // Acumulo suma bloques
    }
		
    arch = readdir(dir);  // Leo siguiente archivo
  }

  write(STDOUT_FILENO, &suma, sizeof(unsigned long));  // Escribo suma en salida estandar
	
  exit(0);
}
