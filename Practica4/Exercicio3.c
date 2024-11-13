#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


//watch -n 0.5 'ps -eLf | grep <nombre-ejecutable>'

void *funcionPthExit(void *arg) {
    int n = *(int *)arg;
    printf("Hilo %d creado\n", n);
    sleep(20);
    printf("Mensaje que no debería aparecer\n");
    pthread_exit(NULL);
}

void *funcionExit(void *arg) {
    int n = *(int *)arg;
    printf("Hilo %d creado\n", n);
    sleep(1);
    printf("El hilo %d va a terminar el proceso\n", n);
    exit(1);
}

int main() {
    pthread_t hilos[3];
    int i = 1, j = 2, z = 3;

    pthread_create(&hilos[0], NULL, funcionPthExit, &i);
    pthread_create(&hilos[1], NULL, funcionPthExit, &j);
    pthread_create(&hilos[2], NULL, funcionExit, &z);

    // Esperar a que los hilos terminen para evitar que el proceso termine antes
    for (int k = 0; k < 3; k++) {
        pthread_join(hilos[k], NULL);
    }

    return 0;
}

