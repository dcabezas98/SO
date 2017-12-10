/*
  Programa que recorre la jerarquía de subdirectorios del
  directorio introducido como argumento, si no se introduce
  argumento, toma como argumento el directorio actual.

  El programa devuelve los pathname y los números de inodo
  de los archivos regulares que tengan permiso de ejecución
  para el grupo y para otros. También devuelve el número de archivos
  en la jerarquía que cumplan la condición y el tamaño total
  ocupado por dichos archivos.

  Recorre la estructura haciendo uso de la función nftw.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <errno.h>

#define criterio(mode) (((mode) & 011) == 011)

static int n = 0;
static int tam = 0;

int count(const char *path, const struct stat* statbuf, int typeflag, struct FTW *ftwbuf){

  if(S_ISREG(statbuf->st_mode) && criterio(statbuf->st_mode)){
    printf("%s %ld \n\n", path, statbuf->st_ino);
    n++;
    tam += (int) statbuf->st_size;
  }

  return 0;
}

int main(int argc, char *argv[]){

  printf("Los inodos son: \n\n");

  if(nftw(argc >= 2 ? argv[1] : ".", count, 20, 0) != 0)
    perror("Error en nftw");

  printf("Existen %d archivos regulares con permiso de ejecución para grupos y otros\n\n", n);

  printf("El tamaño total ocupado por dichos archivos es %d bytes\n\n", tam);
}
