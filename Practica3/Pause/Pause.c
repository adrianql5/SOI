#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

void gestor_SIGURS1(int señal) {
    printf("Hijo: Señal %d recibida. Saliendo del pause.\n", señal);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        exit(1);
    }

    if (pid == 0) {
        signal(SIGUSR1, gestor_SIGURS1);

        printf("Hijo: Esperando señal con pause().\n");
        pause();

        printf("Hijo: Terminando.\n");
        exit(0);
    } else {
        sleep(4);

        printf("Padre: Enviando señal SIGUSR1 al hijo.\n");
        kill(pid, SIGUSR1);

        wait(NULL);
        printf("Padre: El hijo ha terminado.\n");
    }

    return 0;
}

