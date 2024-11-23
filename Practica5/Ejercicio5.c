#include <math.h>
#include <stdio.h>


/*
 * Codigo escrito por
 *
 * Izan Monterroso
 *
 * Adrián Quiroga Linares
 *
 * Tamaño del ejecutable: El programa estático será significativamente más grande.
 *
 * Uso de memoria: El programa dinámico compartirá la biblioteca con otros procesos,
 * mientras que el programa estático cargará su propia copia en memoria.
 *
 * Portabilidad: Un ejecutable estático es más autónomo porque no depende de la
 * presencia de bibliotecas compartidas en el sistema. Sin embargo,
 * será menos eficiente en términos de memoria compartida.
 *
 */


int main() {
    int a = 89;
    double resultado = sin(a); // Calcula el seno de 'a' y almacena el resultado
    printf("El seno de %d es: %f\n", a, resultado);

    printf("Introduce un nuevo valor para 'a': ");
    scanf("%d", &a); // Solicita un nuevo valor para 'a' al usuario

    resultado = sin(a);
    printf("El seno de %d es: %f\n", a, resultado);

    return 0;
}

