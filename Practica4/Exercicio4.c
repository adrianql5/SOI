#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>

#define NUM_HILOS 16
#define NUM_ITERACIONES 100000000

/**
 * Debido a las llamadas frecuentes a sched_yield en los hilos generosos,
 * estos deberían terminar después de los no generosos,
 * ya que liberan la CPU y, por lo tanto,
 * obtienen menos tiempo de procesamiento efectivo.
 */

void* hilo_no_generoso(void* arg) {
    int id = *(int*)arg;
    for (long i = 0; i < NUM_ITERACIONES; i++) {
    }
    printf("Hilo no generoso %d ha terminado.\n", id);
    pthread_exit(NULL);
}

void* hilo_generoso(void* arg) {
    int id = *(int*)arg;
    for (long i = 0; i < NUM_ITERACIONES; i++) {
        sched_yield(); // Ceder CPU a otros hilos
    }
    printf("Hilo generoso %d ha terminado.\n", id);
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_HILOS];
    int ids[NUM_HILOS];

    for (int i = 0; i < NUM_HILOS; i++) {
        ids[i] = i;
        if (i % 2 == 0) {
            pthread_create(&hilos[i], NULL, hilo_no_generoso, &ids[i]);
        } else {
            pthread_create(&hilos[i], NULL, hilo_generoso, &ids[i]);
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    printf("Todos los hilos han terminado.\n");
    return 0;
}

