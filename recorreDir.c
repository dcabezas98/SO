
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define criterio(mode) (((mode) & 011) == 011)

void recorrerDir(char *path, int *n, int *tam){

  struct stat atrib;
  struct dirent *ed;
  char nombre[4096];
  DIR *direct;

  direct = opendir(path);

  if(direct == NULL){
    printf("Error aptertura %s\n", path);
  }

  else{

    ed = readdir(direct);

    while(ed != NULL){

      if(strcmp(ed->d_name, ".") != 0 && strcmp(ed->d_name, "..") != 0){

	sprintf(nombre,"%s/%s",path,ed->d_name);
       
	if(stat(nombre, &atrib) < 0){
	  printf("Error al acceder a atributos de %s\n", nombre);
	  ed = readdir(direct);
	  continue;
	}

	if(S_ISREG(atrib.st_mode) && criterio(atrib.st_mode)){
	  printf("%s %ld \n\n", nombre, atrib.st_ino);
	  (*n)++;
	  (*tam) += (int) atrib.st_size;
	}

	else if(S_ISDIR(atrib.st_mode)){
	  recorrerDir(nombre, n, tam);
	}
      }

      ed = readdir(direct);
    }

    closedir(direct);
  }
}

int main(int argc, char *argv[]){

  printf("Los inodos son: \n\n");

  int n = 0;
  int tam = 0;
  
  if(argc == 2)
    recorrerDir(argv[1], &n, &tam);

  else
    recorrerDir(".", &n, &tam);

  printf("Existen %d archivos regulares con permiso de ejecución para grupos y otros\n\n", n);

  printf("El tamaño total ocupado por dichos archivos es %d bytes\n\n", tam);
}     
