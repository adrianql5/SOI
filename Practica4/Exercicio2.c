#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/**
 * Conclusión
 * Si creamos el hijo desde el hilo3 solo puede ejecutar codigo de ese hijo,
 * y tiene acceso unicamente a ese hilo, no al resto.
 * Si creamos el hijo desde el hilo main, crearia un hijo con todos los hilos que fuera a tener el padre
 */


pid_t hijo;

void *funcion1y2(void *arg) {
    int id = *(int *)arg;
    printf("Hilo %d en ejecución\n", id);
    sleep(5);
    printf("Hilo %d termina\n", id);
    pthread_exit(NULL);
}

void *funcion3(void *arg) {
    printf("Hilo 3 en ejecución y listo para hacer fork\n");

    hijo = fork();
    if (hijo == 0) {//esta parte la ejecuta la parte del hijo del hilo3
        printf("Soy el proceso hijo creado por el hilo 3 (PID: %d)\n", getpid());
        sleep(10);
        exit(0);
    } else if (hijo > 0) {//esta parte la ejecuta la parte del padre del hilo3
        printf("Proceso padre (PID: %d) creó al hijo (PID: %d)\n", getpid(), hijo);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilo1, hilo2, hilo3;
    int id_hilo1 = 1, id_hilo2 = 2, id_hilo3 = 3;


    pthread_create(&hilo1, NULL, funcion1y2, &id_hilo1);
    pthread_create(&hilo2, NULL, funcion1y2, &id_hilo2);
    pthread_create(&hilo3, NULL, funcion3, &id_hilo3);


    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    pthread_join(hilo3, NULL);

    if (hijo > 0) {
        waitpid(hijo, NULL, 0);
    }

    return 0;
}

