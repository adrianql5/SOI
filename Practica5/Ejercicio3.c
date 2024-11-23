#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

/*
 * Código escrito por:
 *
 * Izan Monterroso
 *
 * Adrián Quiroga
 *
 * Podemos apreciar que con cada reserva de memoria,
 * el heap crece hacia abajo aumentadno su rango de direcciones
 * y que al hacer free decrece
 *
 */

int main() {
    printf("Presiona enter para realizar la primera reserva pequeña de memoria\n");
    getchar();


    void *ptr1 = malloc(5);
    if (ptr1 == NULL) {
        printf("Error al realizar malloc para 1 MB.\n");
        return 1;
    }
    printf("Memoria reservada con malloc en la dirección: %p\n", ptr1);

    printf("Presiona Enter para continuar relizar la segunda reserva pequeña de memoria...\n");
    getchar();

    void *ptr2 = malloc(5);
    if (ptr2 == NULL) {
        printf("Error al realizar malloc para 10 MB.\n");
        free(ptr1);
        return 1;
    }
    printf("Memoria reservada con malloc en la dirección: %p\n", ptr2);

    printf("Presiona Enter para continuar y reservar memoria con alloca en la pila...\n");
    getchar();

    void *alloca_ptr = alloca(1024 * 1024); // 1 MB con alloca
    printf("Memoria reservada con alloca(1 MB) en la dirección: %p\n", alloca_ptr);

    printf("Presiona Enter para continuar y hacer un realloc de la region reservada por ptr1...\n");
    getchar();


    printf("Redimensionando la memoria reservada por ptr1 con realloc...\n");
    void *ptr1_realloc = realloc(ptr1, 2 * 1024 * 1024); // Redimensionar a 2 MB
    if (ptr1_realloc == NULL) {
        printf("Error al realizar realloc.\n");
        free(ptr1); // Liberar la memoria original
        free(ptr2);
        return 1;
    }
    printf("Memoria redimensionada con realloc(2 MB) en la dirección: %p\n", ptr1_realloc);

    printf("Presiona Enter para continuar y reservar un tamaño grande de heap...\n");
    getchar();

    double *ptr3 = malloc(1000 * sizeof(double));
    if (ptr3 == NULL) {
        printf("Error al realizar malloc para 1000 doubles.\n");
        free(ptr1_realloc);
        free(ptr2);
        return 1;
    }
    printf("Memoria reservada para 1000 doubles en la dirección: %p\n", ptr3);
    printf("sizeof del puntero devuelto por malloc: %zu bytes\n", sizeof(ptr3));

    printf("Presiona Enter para continuar...\n");
    getchar();

    // Comprobación 5: cambiar dirección del puntero antes de free
    printf("Incrementando el puntero devuelto por malloc antes de llamar a free...\n");
    double *altered_ptr = ptr3 + 1; // Modificar la dirección del puntero
    printf("Nueva dirección del puntero: %p\n", altered_ptr);

    printf("Llamando a free sobre el puntero original...\n");
    free(ptr3); // Siempre debe ser el puntero original de malloc


    free(ptr1_realloc);
    free(ptr2);

    printf("Todas las zonas de memoria liberadas correctamente.\n");
    return 0;
}


