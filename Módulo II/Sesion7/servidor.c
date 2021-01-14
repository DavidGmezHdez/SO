#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define tamano 1024
#define longnombre 50


int main(int argc, char **argv){
    // Variables
    int dfifoe,dfifos; // Descriptores FIFO entrada y salida
    int dfifoproxy; // Descriptor FIFO proxy
    int pid;
    int resultado; 
    char nombrefifoe[longnombre], nombrefifos[longnombre]; // Nombre de los FIFOs entrada y salida
    char nombrefifoproxy[longnombre]; // Nombre del FIFO del proxy
    
    // Comprobación de parámetros
    if (argc < 2) {
        printf ("Servidor: Formato: servidor <nombre_fifo>\n");
        exit(EXIT_FAILURE);
    }
    
    // Creamos los nombres de los FIFOs
    sprintf(nombrefifoe,"%se",argv[1]);
    sprintf(nombrefifos,"%ss",argv[1]);

    // Crear los FIFOs para comunicarme con los clientes  (mkfifo/mknod)
    if(mkfifo(nombrefifoe, S_IRWXU) == -1){
        if(errno == EEXIST){
            printf("Servidor: el fifo %s existe \n",nombrefifoe);
        }
        else{
            perror("Servidor: Error al crear el FIFO de entrada\n");
            exit(-1);
        }
    }
    if(mkfifo(nombrefifos, S_IRWXU) == -1){
        if(errno == EEXIST){
            printf("Servidor: el fifo %s existe \n",nombrefifos);
        }
        else{
            perror("Servidor: Error al crear el FIFO de entrada\n");
            exit(1);
        }
    }

    // Creamos el archivo de bloqueo
    if((creat("bloqueo", S_IRWXU)) == -1)
    {
        printf("\nError %d en creat",errno);										
        perror("\nError en creat de archivo de bloqueo");											
        exit(EXIT_FAILURE);		
    }

    // Abrimos los FIFOs (open)
    if((dfifoe = open(nombrefifoe, O_RDWR)) == -1){
        perror("Servidor: Error al abrir el FIFO de entrada\n");
        exit(-1);
    }
    if((dfifos = open(nombrefifos, O_RDWR)) == -1){
        perror("Servidor: Error al abrir el FIFO de salida\n");
        exit(1);
    }

    printf("Servidor: Los archivos FIFO han sido creados y abiertos correctamente\n");

    // El servidor siempre lee las peticiones del archivo FIFO
    while((resultado = read(dfifoe, &pid, sizeof(int))) != 0){
        // Creamos los procesos de cada cliente (fork)
        if((pid = fork()) == -1){
            perror("Servidor: Error en la creación de un proceso\n");
            exit(1);
        }
        else if(pid == 0){
            // Código del hijo / el que será el proxy
            pid = getpid();
            sprintf(nombrefifoproxy,"fifo.%d",pid);
            // Crear el fichero
            if(mkfifo(nombrefifoproxy, S_IRWXU) == -1){
                if(errno == EEXIST){
                    printf("Servidor: el fifo %s existe \n",nombrefifoproxy);
                }
                else{
                    perror("Servidor: Error al crear el FIFO de proxy\n");
                    exit(-1);
                }
            }

            //Escribimos el PID del proxy
            write(dfifos,&pid,sizeof(int));

            // Lo abrimos
            if((dfifoproxy = open(nombrefifoproxy, O_RDONLY)) == -1){
                perror("Servidor: Error al abrir el FIFO de proxy\n");
                exit(-1);
            }

            //Redireccionamos la entrada estandar
            dup2(dfifoproxy, STDIN_FILENO);
            close(dfifoproxy);

            //Lanzamos el programa PROXY.
            execlp("./proxy","proxy",NULL);
            perror("Servidor: Error al ejecutar el proxy\n");
            exit(1);

        }
    }
}