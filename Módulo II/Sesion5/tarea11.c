// tarea11.c

#include <stdio.h>
#include <signal.h>

int main()

{
sigset_t new_mask;

/* inicializar la nueva mascara de se�ales */
sigemptyset(&new_mask);

sigaddset(&new_mask, SIGUSR1);

/*esperar a cualquier se�al excepto SIGUSR1 */
sigsuspend(&new_mask);

//El control C se lo caga, la orden kill con - SIGINT tambien lo termina, pero como esta ignorando SIGUSR1, kill -SIGUSR1 no hace nada
}