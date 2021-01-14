#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>



int main(int argc, char*argv[]){
    //Variables
    int fd_entrada;             //Descriptores
    int fd_salida;

    struct stat informacion;    // La información del fichero
    int tam;                    //Tamaño del fichero
    char * puntero_entrada;
    char * puntero_salida;

    //Comprobación de argumentos

    if(argc!=3){
        perror("Error en el número de argumentos.");
        printf("El número de argumentos deberia ser 2.\n");
        printf("./s6 <archivo1> <salida>\n");
        exit(-1);
    }


    //Abrimos archivos
    fd_entrada = open(argv[1], O_RDONLY, S_IRUSR | S_IWUSR);

    fd_salida = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    //Vamos a conocer el tamaño del fichero de entrada
    lstat(argv[1], &informacion);
    
    //Obtenemos el tamaño del fichero
    tam = informacion.st_size;

    //Truncamos el archivo destino
    ftruncate(fd_salida,tam);

    //Proyectamos en memoria los ficheros
    puntero_entrada = (char *) mmap(NULL,tam,PROT_READ,MAP_SHARED,fd_entrada,0);
    puntero_salida = (char * ) mmap(NULL,tam,PROT_WRITE,MAP_SHARED,fd_salida,0);

    memcpy(puntero_salida,puntero_entrada,tam);
}