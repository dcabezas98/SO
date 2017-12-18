/*
  Programa que recorre el directorio dado como argumento y escribe
  los nombres de sus archivos en un fifo, crea un hijo que lee el
  nombre y lee el archivo mediante mmap. El hijo añade la cadena
  "REVISADO" al final del documento.
*/

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/mman.h>

static void handler(int sigNum){
  printf("Mi hijo ha finalizado inesperadamente\n");
}

int main(int argc, char *argv[]){

  if(argc < 2){
    perror("\nDebe introducirse un directorio como argumento");
    exit(-1);
  }
  
  DIR *dir;
  struct dirent *arch;
  struct stat st;
  int fd,PID;

  
  chdir(argv[1]);
  dir=opendir(".");
  
  signal(SIGPIPE,handler);

  mkfifo("fifo",S_IRWXU);
    
  if ((fd=open("fifo",O_RDWR))<0){
    perror("error en open.\n");
    exit(-1);
  }

  arch=readdir(dir);
  
  while(arch!=NULL){
    
    if(stat(arch->d_name,&st)<0){
      perror("error en stat.\n");
      exit(-1);
    }
    
    if(S_ISREG(st.st_mode)){

      printf("fork\n");
      if((PID=fork())<0){
	perror("error en fork.\n");
	exit(-1);
      }

      if(PID==0){
	
	char buf[64];
	
	read(fd,buf,64);
	
	int fd2;
	
	if ((fd2=open(buf,O_RDWR))<0){
	  perror("error en open.\n");
	  exit(-1);
	}
	
	struct flock cerrojo;
	
	cerrojo.l_type =F_WRLCK;
	cerrojo.l_whence =SEEK_SET;
	cerrojo.l_start =0;
	cerrojo.l_len =0;
	
	fcntl(fd2,F_SETLKW,&cerrojo);
	
	fstat(fd2,&st);
	
	char t[]="REVISADO";
	
	ftruncate(fd2,st.st_size+strlen(t));
	
	char *memoria;
	
	memoria=(char *) mmap(0,st.st_size+strlen(t),PROT_READ|PROT_WRITE,MAP_SHARED,fd2,0);

	if(memoria == MAP_FAILED){
	  perror("\nError en mmap");
	  exit(-1);
	}
	
	close(fd2);

	printf("\n%s\n",memoria);

	strcat(memoria,t);

	munmap(memoria,st.st_size+strlen(t));

	cerrojo.l_type=F_UNLCK;

	fcntl(fd2,F_SETLK,&cerrojo);

	exit(0);
      }
      else{

	printf("Nombre de archivo: %s. El número de inodo es %lu y el UID del propietario es %lu.\n", arch->d_name,st.st_ino,st.st_uid);

	if(!(st.st_mode & S_IRUSR)){
	  chmod(arch->d_name,st.st_mode|S_IRUSR);
	  printf("Asignando permisos a %s.\n",arch->d_name);
	}

	write(fd,arch->d_name,strlen(arch->d_name)+1);

	wait(NULL);
      }
    }
    arch=readdir(dir);
  }

  close(fd);
  unlink("fifo");

  exit(0);
}				
