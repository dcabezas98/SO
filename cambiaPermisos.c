
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

  struct stat atrib;
  struct dirent *ed;
  char nombre_archivo[100];
  DIR *direct;
  long perm;

  if(argc == 3){

    direct = opendir(argv[1]);
    perm = strtol(argv[2],NULL,8);
    
    if(direct == NULL){
      printf("Error apertura %s\n",argv[1]);
      exit(-1);
    }
  } else{

    printf("Uso: ./cambiaPermisos <pathname> <permisos>\n");
    exit(-1);
  }

  if(chdir(argv[1]) < 0){
    printf("Error al acceder al directorio %s\n",argv[1]);
    exit(-1);
  }
  
  ed = readdir(direct);

  umask(0);

  while(ed != NULL){

    if(stat(ed->d_name,&atrib) < 0) {
      printf("Error al acceder a atributos de %s\n",ed->d_name);
      exit(-1);
    }

    if(S_ISREG(atrib.st_mode)){

      printf("%s: %o ", ed->d_name, atrib.st_mode);

      if(chmod(ed->d_name,perm) < 0)
	printf("Error: %d\n", errno);

      else{
	stat(ed->d_name,&atrib);
	printf("%o \n", atrib.st_mode);
      }

    }
    ed = readdir(direct);
  }

  closedir(direct);
}

  
