/*
  Programa que imita el funcionamiento de la orden cp, deben
  intrucirse el nombre del archivo regular a copiar y el path
  del archivo de salida, si tiene el mismo nombre y está en la
  misma carpeta, se añade la cadena "(copy)" al final del
  nombre.

  El programa proyecta el archivo de origen en una región de memoria,
  luego crea el archivo de destino, lo hace el mismo tamaño con
  ftruncate y también lo proyecta en memoria. Copia con memcpy la memoria
  de una región a otra y finalmente destruye las proyecciones con 
  mmunmap.
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]){

  int fd1, fd2;

  char *origin;
  char	*destiny;
  char copia[1024];
  struct stat atrib;

  if(argc < 2){
    printf("Debe introducirse el archivo a copiar y opcionalmente el nombre del fichero de salida\n");
    exit(-1);
  }

  if(argc == 3)
    strcpy(copia, argv[2]);

  else
    sprintf(copia, "%s(copy)", argv[1]);

  if((fd1 = open(argv[1], O_RDONLY)) < 0){
    perror("\nError en open del original");
    exit(-1);
  }

  if(fstat(fd1, &atrib) == -1){
    perror("\nError en fstat");
    exit(-1);
  }

  if(!S_ISREG(atrib.st_mode)){
    printf("\nEl archivo no es regular\n");
    exit(-1);
  }

  umask(0);
  if((fd2 = open(copia, O_RDWR | O_CREAT | O_TRUNC, atrib.st_mode)) < 0){
    perror("\nError en open de la copia");
    exit(-1);
  }

  ftruncate(fd2, atrib.st_size);

  origin = (char*) mmap(NULL, atrib.st_size, PROT_READ, MAP_SHARED, fd1, 0);
  if(origin == MAP_FAILED){
    perror("\nFallo en proyección del original");
    exit(-1);
  }
  close(fd1);

  destiny = (char*) mmap(NULL, atrib.st_size, PROT_WRITE, MAP_SHARED, fd2, 0);
  if(destiny == MAP_FAILED){
    perror("\nFallo en proyección de la copia");
    exit(-1);
  }
  close(fd2);

  memcpy(destiny, origin, atrib.st_size);

  if(munmap(origin, atrib.st_size) == -1){
    perror("\nError al eliminar la proyección del original");
    return -1;
  }

  if(munmap(destiny, atrib.st_size) == -1){
    perror("\nError al eliminar la proyección de la copia");
    return -1;
  }

  return EXIT_SUCCESS;
}
