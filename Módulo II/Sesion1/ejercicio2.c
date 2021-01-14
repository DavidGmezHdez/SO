

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	/* Primitive system data types for abstraction\
													 of implementation-dependent data types.
													 POSIX Standard: 2.6 Primitive System Data Types
													 <sys/types.h>
												*/
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include <string.h>

#define TAM_CADENA_ID_BLOQUE 12
#define TAM_CADENA_VAN_BYTES 67
#define TAM_BLOQUE 80

int main(int argc, char *argv[]) {
	int fd_archivo, fd_salida;								//Descriptores de los archivos a utilizar

	char cadenaBloque[TAM_CADENA_ID_BLOQUE];	//Array para cada cadena "Bloque 1", "Bloque 2", etc. al comienzo de cada bloque
	char bloque_80[TAM_BLOQUE];								//Array para cada bloque leído de archivo

	int bytesLeidos;													//Bytes leídos en cada acceso a archivo
	int numBloques;								//Contador del número de bloques que se llevan leídos

	if (argc < 2) { //Leemos de la entrada de teclado
		fd_archivo=STDIN_FILENO;
	}
	else {
		// Abrimos archivo argumento
		if( (fd_archivo=open(argv[1],O_RDONLY))<0) {
			printf("\nError %d en open de %s",errno, argv[1]);
			perror("\nError en open");
			exit(EXIT_FAILURE);
		}
	}

	// Abrimos archivo salida.txt
	if( (fd_salida=open("salida.txt",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR))<0) {
		printf("\nError %d en open de salida.txt",errno);
		perror("\nError en open");
		exit(EXIT_FAILURE);
	}

	numBloques=0;
	while ((bytesLeidos=read(fd_archivo, bloque_80, TAM_BLOQUE)) > 0) {
		numBloques++;
		
		sprintf(cadenaBloque,"\nBloque %d\n",numBloques); //Añade '\0' al final de la cadena
	
		if(write(fd_salida,cadenaBloque,strlen(cadenaBloque)) != strlen(cadenaBloque)) {
			perror("\nError en write de cadenaBloque");
			exit(EXIT_FAILURE);
		}

		if(write(fd_salida,"//Aqui van los primeros 80 bytes del archivo pasado como argumento\n",strlen("//Aqui van los primeros 80 bytes del archivo pasado como argumento\n")) != strlen("//Aqui van los primeros 80 bytes del archivo pasado como argumento\n")) {
			perror("\nError en write de \"Aquí van bytes\"");
			exit(EXIT_FAILURE);
		}
	
		if(write(fd_salida,bloque_80,bytesLeidos) < 0) {
			perror("\nError en write de bloque");
			exit(EXIT_FAILURE);
		}
	}

	close(fd_archivo);
	close(fd_salida);
	return EXIT_SUCCESS;
}