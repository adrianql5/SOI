#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int variableGlobal = 45;

void *funcionHilo(void *arg) {
    int n = *(int *)arg;
    printf("El hilo recibe la variable local: %d\n", n);
    n = 145234;
    printf("Nuevo valor de la variable local cambiada por el hilo: %d\n", n);

    printf("Valor de la variable global en el hilo: %d\n", variableGlobal);
    variableGlobal = 2384378;
    printf("Nuevo valor de la variable global en el hilo: %d\n", variableGlobal);

    pthread_exit(NULL);
}

int main() {
    pthread_t hilo;
    int variableLocal = 34;

    printf("Valor de la variable local en el hilo principal: %d\n", variableLocal);
    printf("Valor de la variable global en el hilo principal: %d\n", variableGlobal);

    pthread_create(&hilo, NULL, funcionHilo, &variableLocal);
    pthread_join(hilo, NULL);  // Espera a que el hilo termine

    printf("Valor de la variable local en el hilo principal después del hilo: %d\n", variableLocal);
    printf("Valor de la variable global en el hilo principal después del hilo: %d\n", variableGlobal);

    return 0;
}

