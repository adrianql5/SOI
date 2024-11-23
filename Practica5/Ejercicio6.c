#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int b = 10;

void *gestorHilo1(void *arg) {
    int param = *(int *)arg;
    int local_var = param * b;

    int *dynamic_var = (int *)malloc(sizeof(int));
    *dynamic_var = local_var;

    printf("Hilo %lu:\n", pthread_self());
    printf("  Dirección de la variable global (b): %p\n", (void *)&b);
    printf("  Dirección del parámetro recibido (param): %p\n", (void *)arg);
    printf("  Dirección de la variable local (local_var): %p\n", (void *)&local_var);
    printf("  Dirección de la memoria dinámica (dynamic_var): %p\n\n", (void *)dynamic_var);

    free(dynamic_var);

    pthread_exit(NULL);
}

int main() {
    pthread_t hilo1, hilo2;

    int a = 9;

    printf("Hilo principal:\n");
    printf("  Dirección de la variable global (b): %p\n", (void *)&b);
    printf("  Dirección de la variable local (a): %p\n\n", (void *)&a);

    pthread_create(&hilo1, NULL, gestorHilo1, (void *)&a);
    pthread_create(&hilo2, NULL, gestorHilo1, (void *)&a);

    scanf("%d",&a);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}

