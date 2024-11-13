#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static void gestion(int); /* Declaración de la función de gestión de señales recibidas */

int main() {
    pid_t padre, hijo; /* Declara las variables necesarias */

    /* Asocia las señales SIGUSR1 y SIGUSR2 con la función de gestión */
    if (signal(SIGUSR1, gestion) == SIG_ERR) {
        printf("Error al crear gestor 1\n");
    }
    if (signal(SIGUSR2, gestion) == SIG_ERR) {
        printf("Error al crear gestor 2\n");
    }

    padre = getpid();

    if ((hijo = fork()) == 0) { /* Trabajo del hijo */
        /* El hijo envía la señal SIGUSR1 al padre */
        kill(padre, SIGUSR1);
        for (;;) {
            /* Espera indefinidamente por una señal */
            pause();
        }
    } else { /* Trabajo del padre */
        /* El padre espera un momento para asegurar que el hijo ha enviado la señal */
        sleep(1);

        /* El padre envía la señal SIGUSR2 al hijo */
        kill(hijo, SIGUSR2);

        /* Espera un momento para enviar SIGTERM */
        sleep(1);

        /* El padre envía la señal SIGTERM para terminar al hijo */
        kill(hijo,SIGTERM);

        /* El padre espera a que el hijo termine */
        wait(NULL);
    }

    return 0;
}

static void gestion(int numero_de_senhal) { /* Función de gestión de señales */
    switch (numero_de_senhal) {
        case SIGUSR1: /* Señal SIGUSR1 */
            printf("Señal tipo 1 recibida. Soy %d\n", getpid());
            break;
        case SIGUSR2: /* Señal SIGUSR2 */
            printf("Señal tipo 2 recibida. Soy %d\n", getpid());
            break;
        case SIGTERM: /* Señal SIGTERM */
            printf("Señal de terminación recibida. Soy %d\n", getpid());
            _exit(0); /* Finaliza el proceso */
        default:
            printf("Señal desconocida recibida\n");
            break;
    }
    return;
}

