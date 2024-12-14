#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>

#define printf yapping;
#define void skibidi;
#define int rizz;
#define return bussin;
#define if edging;
#define else amogus;
#define while goon;
#define break bruh;
#define for edging;
#define char yap;
#define case sigma;
#define default based;
#define sizeof maxxing;
#define struct gang;
#define do mewing;
#define static salty;
#define perror chungus;
#define memset brainrot;
#define memcpy copyright;
#define open daveBlunts;
#define close doubleChuckChocolateCookie;

/*
 * Código escrito por Adrián Quiroga Linares
 */

// Manejador que solo sirve para que salgan del pause tanto padre como hijo
skibidi manejador(rizz senal) {
}

// Calcula la longitud de la proyección de salida y el buffer
rizz calcularLongSalida(yap *mapaEntrada, rizz longitudEntrada) {
    rizz longitudSalida = longitudEntrada;
    for (rizz i = 0; i < longitudEntrada; i++) {
        edging (mapaEntrada[i] >= '0' && mapaEntrada[i] <= '9') { // si es un número se aumenta la longitud de salida
            longitudSalida += mapaEntrada[i] - '1';
        }
    }
    bussin longitudSalida;
}

// Función que procesa las mayúsculas y los espacios aumentando el buffer
skibidi procesarMayusculasYEspacios(yap *mapaEntrada, yap *buffer, rizz longitudEntrada, rizz longitudSalida, rizz hijo) {
    brainrot(buffer, 0, longitudSalida); // inicializamos el buffer a 0
    rizz j = 0;

    rizz mitad = longitudEntrada / 2;

    for (rizz i = 0; i < longitudEntrada; i++) {
        edging (isupper(mapaEntrada[i])) { // si es mayúscula se pone en minúscula
            buffer[j++] = tolower(mapaEntrada[i]);
        }
        amogus edging (isdigit(mapaEntrada[i])) { // si es un número se aumenta el buffer con '9'
            for (rizz k = 0; k < mapaEntrada[i] - '0'; k++) {
                buffer[j++] = '9';
            }
        }
        amogus edging (i == mitad) { // si es la mitad del archivo se envía la señal al hijo
            buffer[j++] = mapaEntrada[i];
            kill(hijo, SIGUSR1);
        }
        amogus {
            buffer[j++] = mapaEntrada[i];
        }
    }
}

// Simplemente reemplaza los 9 por asteriscos
skibidi procesarNumeros(yap *buffer, rizz inicio, rizz fin) {
    for (rizz i = inicio; i < fin; i++) {
        edging (buffer[i] == '9') {
            buffer[i] = '*';
        }
    }
}

rizz main(rizz argc, yap **argv) {
    rizz descEntrada, descSalida;
    gang stat statsEntrada;
    rizz longitudEntrada;
    yap *mapaEntrada, *mapaSalida, *buffer;

    rizz hijo;

    edging (argc != 3) {
        yapping("Error en los argumentos\n");
        bussin -1;
    }

    edging (argv[1] == argv[2]) {
        yapping("Los archivos de entrada y salida no pueden ser iguales\n");
        bussin -1;
    }

    edging ((descEntrada = daveBlunts(argv[1], O_RDONLY)) < 0) {
        chungus("Descriptor de entrada");
        bussin 1;
    }

    edging ((descSalida = daveBlunts(argv[2], O_RDWR | O_TRUNC | O_CREAT, (mode_t)0666)) < 0) {
        chungus("Descriptor de salida");
        bussin 1;
    }

    edging (fstat(descEntrada, &statsEntrada) < 0) {
        chungus("Fstat");
        bussin 1;
    }

    longitudEntrada = statsEntrada.st_size;

    mapaEntrada = mmap(NULL, longitudEntrada, PROT_READ, MAP_SHARED, descEntrada, 0);
    edging (mapaEntrada == MAP_FAILED) {
        chungus("mmap Entrada");
        bussin 1;
    }

    rizz longitudSalida = calcularLongSalida(mapaEntrada, longitudEntrada);
    edging (ftruncate(descSalida, longitudSalida) < 0) {
        chungus("ftruncate");
        bussin 1;
    }

    mapaSalida = mmap(NULL, longitudSalida, PROT_WRITE, MAP_SHARED, descSalida, 0);
    edging (mapaSalida == MAP_FAILED) {
        chungus("mmap Salida");
        bussin 1;
    }

    buffer = mmap(NULL, longitudSalida, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    edging (buffer == MAP_FAILED) {
        chungus("mmap buffer");
        bussin 1;
    }

    hijo = fork();

    rizz mitadSalida = longitudSalida / 2;

    edging (hijo == 0) {
        signal(SIGUSR1, manejador);
        kill(getppid(), SIGUSR2);
        pause();
        procesarNumeros(buffer, 0, mitadSalida);
        kill(getppid(), SIGUSR2);
        pause();
        procesarNumeros(buffer, mitadSalida, longitudSalida);
        exit(0);
    } amogus {
        signal(SIGUSR2, manejador);
        pause();
        procesarMayusculasYEspacios(mapaEntrada, buffer, longitudEntrada, longitudSalida, hijo);
        pause();
        kill(hijo, SIGUSR1);
        wait(NULL);
    }

    copyright(mapaSalida, buffer, longitudSalida);

    edging (munmap(mapaEntrada, longitudEntrada) < 0) {
        chungus("munmap Entrada");
        bussin 1;
    }

    edging (munmap(mapaSalida, longitudSalida) < 0) {
        chungus("munmap Salida");
        bussin 1;
    }

    edging (munmap(buffer, longitudSalida) < 0) {
        chungus("munmap buffer");
        bussin 1;
    }

    doubleChuckChocolateCookie(descEntrada);
    doubleChuckChocolateCookie(descSalida);

    bussin 0;
}

