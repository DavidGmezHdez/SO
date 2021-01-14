#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>

int main (int argc, char *argv[]){
    DIR *direct;
    char *pathname;
    struct dirent *elemento_d;
    extern int errno;

    if(argc == 2){
        // Le paso el directorio
        pathname = argv[1];
        //Abor el directorio
        direct = opendir(pathname);
    }

    while((elemento_d=readdir(direct)) !=NULL){
        if(strcmp(elemento_d->d_name,".") != 0 && strcmp(elemento_d->d_name,"..") != 0)
            printf("El directorio es: %s y su elemento es: %s\n",pathname, elemento_d->d_name);
    }
}

/*
if(strcmp(fichero->d_name,".") != 0 && (strcmp(fichero->d_name,"..") != 0

Para evirtar que lea el directorio actual y el del padre
*/