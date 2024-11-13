#include <signal.h>  // Biblioteca para el manejo de señales.
#include <stdio.h>   // Biblioteca estándar de entrada/salida.
#include <unistd.h>  // Biblioteca para llamadas al sistema (como fork, sleep, etc.).
#include <stdlib.h>  // Biblioteca para funciones de utilidad (como exit).
#include <sys/wait.h> // Biblioteca para funciones de espera de procesos.

/**
 * Código creado por:
 * Adrián Quiroga Linares
 * Manuel Pérez Pazos
 */

// Gestor de las señales
static void gestor(int señal) {
    switch (señal) {
        case SIGUSR1:
            printf("Se acaba de recibir la señal SIGUSR1\n");
            break;

        case SIGUSR2:
            printf("Se acaba de recibir la señal SIGUSR2\n");
            break;

        case SIGINT:
            printf("Se acaba de recibir la señal SIGINT y será ignorada\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    pid_t hijo1, hijo2, padre;
    sigset_t bloqueo, pendientes;  // Conjuntos de señales para bloquear y verificar señales pendientes
    struct sigaction sa;  // Estructura para manejar las acciones de las señales

    padre = getpid();
    printf("PID del padre: %d\n", padre);

    // Configuración del gestor de señales
    sa.sa_handler = gestor;  // Asignamos la función `gestor` como manejador de señales
    sa.sa_flags = SA_RESTART;  // Reiniciamos ciertas llamadas al sistema interrumpidas por señales
    sigemptyset(&sa.sa_mask);  // Limpiamos la máscara de señales

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    // Inicializamos el conjunto de señales de bloqueo y añade SIGUSR1
    sigemptyset(&bloqueo);
    sigaddset(&bloqueo, SIGUSR1);
    sigprocmask(SIG_BLOCK, &bloqueo, NULL);  // Bloqueamos la señal SIGUSR1
    printf("Se acaba de bloquear SIGUSR1.\n");

    hijo1 = fork();
    if (hijo1 < 0) {
        printf("Error al crear el primer hijo\n");
        exit(EXIT_FAILURE);
    } else if (hijo1 == 0) {
        printf("PID del primer hijo H1: %d\n", getpid());

        // El hijo 1 envía la señal SIGUSR1 al padre
        kill(padre, SIGUSR1);
        printf("H1 envía SIGUSR1 al padre.\n");

        sleep(20);  // El hijo duerme por 20 segundos

        // El hijo 1 envía la señal SIGUSR2 al padre
        kill(padre, SIGUSR2);
        printf("H1 envía SIGUSR2 al padre.\n");

        sleep(20);  // El hijo duerme por otros 20 segundos

        exit(42);
    }

    hijo2 = fork();
    if (hijo2 < 0) {
        printf("Error al crear el segundo hijo\n");
        exit(EXIT_FAILURE);
    } else if (hijo2 == 0) {
        printf("PID del segundo hijo H2: %d\n", getpid());

        sleep(5);  // El hijo duerme por 5 segundos

        // El hijo 2 envía la señal SIGINT al padre
        kill(padre, SIGINT);
        printf("H2 envía SIGINT al padre.\n");

        exit(0);  // Termina con el código de salida 0
    }

    // Dormimos al padre para permitir que H1 envíe SIGUSR2
    sleep(25);
    printf("El padre despierta después de dormir.\n");

    // Verificamos si la señal SIGUSR1 está pendiente
    sigpending(&pendientes);
    if (sigismember(&pendientes, SIGUSR1)) {
        printf("La señal SIGUSR1 está pendiente.\n");
    }

    // Desbloqueamos la señal SIGUSR1 para que sea procesada
    printf("Señal SIGUSR1 desbloqueada.\n");
    sigprocmask(SIG_UNBLOCK, &bloqueo, NULL);

    // Esperamos a que el primer hijo termine y verificamos su código de salida
    int status;
    waitpid(hijo1, &status, 0);
    if (WIFEXITED(status)) {
        printf("El primer hijo H1 terminó con código de salida %d.\n", WEXITSTATUS(status));
    }

    waitpid(hijo2, NULL, 0);

    return EXIT_SUCCESS;
}
