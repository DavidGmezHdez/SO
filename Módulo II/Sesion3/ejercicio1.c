#include <unistd.h>
#include <stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

	if(argc <2){
        printf("Error en los argumentos");
    }

    int numero = argv[1];

    if (numero%4 == 0)
        printf("El número es divisible por 4\n");
    else
        printf("El número no es divisible por 4\n");
    
	
	fork(); //Creamos proceso. En este caso id_proceso ya sería diferente
	if (numero%2 == 0)
        printf("El número es par\n");
    else
        printf("El número es impar\n");

}
