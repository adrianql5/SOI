#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>


/** Codigo escrito por
 * Adrián Quiroga Linares
 * Manuel Pérez Pazos
 */



pid_t *arrayPIDs;  //Array que almacena los PIDs
int n;  // Número de hijos
pid_t primerhijo;  // PID del primer hijo

// Función para recolocar el array cuando un hijo es eliminado
void recolocar_array(int indice) {
    pid_t temp = arrayPIDs[indice];  // Guardamos el PID eliminado
    for (int i = indice; i < n - 1; i++) {
        arrayPIDs[i] = arrayPIDs[i + 1];  // Movemos los elementos a la izquierda
    }
    arrayPIDs[n - 1] = temp;  // Colocamos el PID eliminado al final del array
}

// Gestor de la señal SIGUSR2
void gestor_SIGUSR2(int s) {
    printf("Proceso %d en el gestor\n", getpid());
    pid_t anterior, actual;

    for (int i = 1; i < n; i++) {  // Recorremos desde el segundo hijo (i = 1)
        actual = arrayPIDs[i];  // PID del hijo actual
        anterior = arrayPIDs[i - 1];  // PID del hijo anterior

        if (actual == 0) {  // Verificamos si este es el proceso que recibió la señal
            if (anterior != primerhijo) {
                printf("El hijo H[%d] mató a H[%d] (PID: %d)\n", i, i - 1, anterior);
                kill(anterior, SIGTERM);  // El proceso actual mata al anterior
                recolocar_array(i - 1);  // Recolocamos el array
                n--;  // Reducimos el número total de hijos activos
                printf("Ahora el hijo H[%d] pasa a ser el hijo H[%d]\n", i, i-1);
            } else {
                printf("El primer hijo (PID: %d) no puede matar a nadie!\n", primerhijo);
            }
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    // Asignamos el manejador de la señal SIGUSR2
    signal(SIGUSR2, gestor_SIGUSR2);

    printf("¿Cuántos hijos quieres crear? ");
    scanf("%d", &n);
       arrayPIDs = malloc(sizeof(pid_t) * n);

    // Creamos los n hijos
    for (int i = 0; i < n; i++) {
        arrayPIDs[i] = fork();  // Creamos un nuevo proceso hijo
        if (arrayPIDs[i] == 0) {  // Si es el proceso hijo
            printf("Hijo H[%d] creado, PID: %d\n", i, getpid());
            while (1) sleep(1);  // Mantenemos al proceso hijo en espera
            return 0;          }
    }

    primerhijo = arrayPIDs[0];  // Guardamos el PID del primer hijo

    // El padre espera a que el primer hijo termine
    waitpid(primerhijo, NULL, 0);

    printf("El hijo H[0] ha acabado, se procederá a la eliminación del resto de procesos no eliminados\n");
        for (int i = 0; i < n; i++) {
        kill(arrayPIDs[i], SIGTERM);
    }

       free(arrayPIDs);

    return 0;
}

