#include <unistd.h>
#include <stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

	pid_t id_proceso;
	
	fork(); //Creamos proceso. En este caso id_proceso ya sería diferente
	id_proceso = getpid();

	pritnf("ID del proceso: %d\n", id_proceso);

}
