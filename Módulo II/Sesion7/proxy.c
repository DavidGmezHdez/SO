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
/*
    1) Leer el contenido del FIFO proxy (entrada estandatr)
    2) Almacenarlo en su archivo temporal
    3) Bloquear en pantalla e imprimir
    4) Borrar archivos residuales
*/

int main(int argc, char **argv){
    int resultado,dfbloqueo; 
    char buffer[tamano];
    struct flock cerrojo; // Inicializamos el cerrojo para bloquear todo el archivo 
    FILE *temp = tmpfile();
    int nbytes;
    char nombrefifoproxy[longnombre]; // Nombre del FIFO del proxy

    while((resultado = read(STDIN_FILENO, buffer, tamano)) > 0){ 
        fwrite(buffer, sizeof(char), resultado, temp); 
    }

    //Abrimos el cerrojo
    if ((dfbloqueo = open("bloqueo", O_RDWR)) == -1){
        printf("Proxy: Error al abrir blockfile\n");
        exit(1);
    }
        

    //Bloqueamos el cerrojo
    cerrojo.l_type= F_WRLCK; 
    cerrojo.l_whence= SEEK_SET; 
    cerrojo.l_start= 0; 
    cerrojo.l_len = 0; 
    //Si vamos a bloquearlo y ya lo esta, entonces el proceso duerme 
    if (fcntl(dfbloqueo, F_SETLKW, &cerrojo) == -1){ 
        perror ("Proxy: problemas al bloquear para impresion"); 
        exit(1); 
    }

    // Imprimimos el archivo temporal

    while(!feof(temp)){ 
        fread(buffer, sizeof(char), 1024, temp);
        // Escribir en la salida estandar. 
        write(STDOUT_FILENO, buffer, 1024); 
    }

    // Desbloqueamos el cerrojo
    cerrojo.l_type= F_UNLCK; 

    // Borramos archivos residuales
    sprintf(nombrefifoproxy,"fifo.%d", getpid()); 
    unlink(nombrefifoproxy);
    exit(0);

}