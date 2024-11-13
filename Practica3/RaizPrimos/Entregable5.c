#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

/**
 * Código escrito por:
 * Adrián Quiroga Linares
 * Manuel Pérez Pazos
 */

double r = 0;

void manejador(int s) {
    if (s == SIGUSR1) {
        kill(getpid(), SIGTERM);
    }
    if (s == SIGALRM) {
        printf("La última raíz calculada dio como resultado %f\n", r);
        alarm(1);  // Reinicia la alarma para que se dispare de nuevo en 1 segundo
    }
}

int esprimo(int n) {
    int c;
    if (n <= 1) return 0;  // Números menores o iguales a 1 no son primos
    for (c = 2; c <= n / 2; c++) {  // Itera desde 2 hasta la mitad de n
        if (esprimo(c) == 1) {  // Verifica si c es primo
            if (n % c == 0) {  // Si n es divisible por c, no es primo
                return 0;
            }
        }
    }
    return 1;  // Si no se encontró un divisor, n es primo
}

void main() {
    double n = 1;
    printf("El programa tiene PID %d\n", getpid());

    if (signal(SIGUSR1, manejador) == SIG_ERR) {
        exit(1);
    }
    if (signal(SIGALRM, manejador) == SIG_ERR) {
        exit(1);
    }

    alarm(1);  // Establece una alarma que enviará SIGALRM en 1 segundo

    while (1) {
        if (esprimo((int)n) == 1) {
            r = sqrt(n);
        }
        n++;
    }
}

