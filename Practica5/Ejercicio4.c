#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Código escrito por:
 *
 * Izan Monterroso
 *
 * Adrián Quiroga
 *
 * Esperamos ver que el hijo y el padre tienen regiones
 * de memoria virtual iguales. Y que después del cambio de imagen
 * ocupan diferentes y que el heap a desaparecido porque el proceso
 * al que llama al execv no realiza ningún malloc.
 *
 * Tambien podemos ver que el padre no tiene heap porque no se realiza
 * la reserva de memoria en el.
 *
 */

int main() {
    pid_t hijo;
    hijo = fork();

    if (hijo == 0) {

        printf("Presiona una tecla para continuar y hacer el malloc en el hijo ...\n");
        getchar();

        void *a;
        a = (void*) malloc(1024* 1024*10);
        printf("Direccion de memoria de 'a' en el hijo antes de execv: %p\n", a);

        printf("Presiona una tecla para continuar...\n");
        getchar();

        char *arr[] = { NULL };
        execv("/home/adrianql/Escritorio/SOI/Practica5/Ejercicio4_2", arr);
    }

    waitpid(hijo, NULL, 0);

    return 0;
}

