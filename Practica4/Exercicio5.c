#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>


/*
 * Código escrito por:
 *
 * Pablo Mouriño Lorenzo, Adrián Quiroga Linares
 *
 * Podemos apreciar que a medida que aumentamos el número de hilos, el tiempo de ejecución se reduce rápidamente.
 * Sin embargo, tras superar el máximo de cores de esta máquina (16), el tiempo de reducción comienza a disminuir
 * de manera menos pronunciada, acercándose a un comportamiento asintótico.
 *
 * Esto se debe a que, una vez alcanzado el número de cores disponibles, los hilos adicionales deben compartir los
 * mismos recursos, lo que implica un cambio de contexto más frecuente y no contribuye de manera significativa a la
 * eficiencia.
 *
 * Tiempo con 1 Hilo: 3.52 segundos
 * Tiempo con 2 Hilos: 3.32 segundos
 * Tiempo con 16 Hilos: 2.16 segundos
 * Tiempo con 50 Hilos: 0.88 segundos
 * Tiempo con 100 Hilos: 0.65 segundos
 * Tiempo con 10000 Hilos: 0.60 segundos
 */



#define NHILOS 16
#define NITERA 1000000000


//Array global que almacena los resultados de las sumas parciales
double sum_parcial[NHILOS] = {0.0};


//Funcion que calcula la suma parcial de cada Hilo
void *calcular_parcial(void *arg) {
    int id = *(int *)arg;
    printf("Hilo %d\n",id);
    //Si es el hilo 0, calcula desde 1 hasta 10, y en funcion del numero de Hilos, pongamos 2, salta hasta 21 y así sucesivamentes
    for (long i = id * 10 + 1; i <= NITERA; i += NHILOS * 10) {
        for (long j = i; j < i + 10 && j <= NITERA; ++j) {
            sum_parcial[id] += 1.0 / (j * j);//modificamos el valor del array de sumas parciales
        }
    }
    pthread_exit(NULL);
}


int main() {
    pthread_t hilos[NHILOS];
    int ids[NHILOS];
    double sum_total_paralela = 0.0, sum_total_secuencial = 0.0;
    double pi2_6 = M_PI * M_PI / 6.0;//valor exacto para realizar comprobaciones
    struct timeval inicio, fin;

    gettimeofday(&inicio, NULL);
    //Bucle para crear los hilos
    for (int i = 0; i < NHILOS; i++) {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, calcular_parcial, (void *)&ids[i]);
    }

    for (int i = 0; i < NHILOS; i++) {
        pthread_join(hilos[i], NULL);
        sum_total_paralela += sum_parcial[i];//calculamos la suma paralela
    }
    gettimeofday(&fin, NULL);
    double tiempo_paralelo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_usec - inicio.tv_usec) / 1000000.0;


    gettimeofday(&inicio, NULL);
    for (long i = 1; i <= NITERA; i++) {
        sum_total_secuencial += 1.0 / (i * i);//calculamos la suma secuencial
    }
    gettimeofday(&fin, NULL);
    double tiempo_secuencial = (fin.tv_sec - inicio.tv_sec) + (fin.tv_usec - inicio.tv_usec) / 1000000.0;

    //Mostramos por pantalla los resultados
    printf("Resultado paralelo: %.15f\n", sum_total_paralela);
    printf("Resultado secuencial: %.15f\n", sum_total_secuencial);
    printf("Diferencia entre paralelo y secuencial: %.15f\n", fabs(sum_total_paralela - sum_total_secuencial));
    printf("Diferencia entre valor paralelo y valor exacto (pi^2/6): %.15f\n", fabs(sum_total_paralela - pi2_6));
    printf("Diferencia entre valor secuencial y valor exacto (pi^2/6): %.15f\n", fabs(sum_total_secuencial - pi2_6));
    printf("Tiempo paralelo: %.2f segundos\n", tiempo_paralelo);
    printf("Tiempo secuencial: %.2f segundos\n", tiempo_secuencial);
    printf("Diferencia de tiempo entre paralelo y secuencial: %.2f\n", fabs(tiempo_paralelo-tiempo_secuencial));
    printf("Ahorro de tiempo usando hilos: %.2f%%\n", 100 * (tiempo_secuencial - tiempo_paralelo) / tiempo_secuencial);

    return 0;
}

