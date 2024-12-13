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

void manejador(int senal) {
}

pid_t hijo;

int calcularLongSalida(char *mapaEntrada, int longitudEntrada) {
    int longitudSalida = longitudEntrada;
    for (int i = 0; i < longitudEntrada; i++) {
        if (mapaEntrada[i] >= '0' && mapaEntrada[i] <= '9') {
            longitudSalida += mapaEntrada[i] - '1';
        }
    }
    return longitudSalida;
}

void procesarMayusculasYEspacios(char *mapaEntrada, char *buffer, int longitudEntrada, int longitudSalida) {
    memset(buffer, 0, longitudSalida);
    int j = 0;

    for (int i = 0; i < longitudEntrada; i++) {
        if (isupper(mapaEntrada[i])) {
            buffer[j++] = tolower(mapaEntrada[i]);
        } else if (isdigit(mapaEntrada[i])) {
            for (int k = 0; k < mapaEntrada[i] - '0'; k++) {
                buffer[j++] = '9';
            }
        } else {
            buffer[j++] = mapaEntrada[i];
        }

        if (i == (longitudEntrada / 2) - 1) {

            kill(hijo, SIGUSR1);
        }
    }

    kill(hijo, SIGUSR1);
}

void procesarNumeros(char *buffer, int inicio, int fin) {
    for (int i = inicio; i < fin; i++) {
        if (buffer[i] == '9') {
            buffer[i] = '*';
        }
    }
}

int main(int argc, char **argv) {
    int descEntrada, descSalida;
    struct stat statsEntrada;
    int longitudEntrada;
    char *mapaEntrada, *mapaSalida, *buffer;

    if (argc != 3) {
        printf("Error en los argumentos\n");
        return -1;
    }

    if ((descEntrada = open(argv[1], O_RDONLY)) < 0) {
        perror("Descriptor de entrada");
        return 1;
    }

    if ((descSalida = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, (mode_t)0666)) < 0) {
        perror("Descriptor de salida");
        return 1;
    }

    if (fstat(descEntrada, &statsEntrada) < 0) {
        perror("Fstat");
        return 1;
    }

    longitudEntrada = statsEntrada.st_size;
    mapaEntrada = mmap(NULL, longitudEntrada, PROT_READ, MAP_SHARED, descEntrada, 0);
    if (mapaEntrada == MAP_FAILED) {
        perror("mmap Entrada");
        return 1;
    }

    int longitudSalida = calcularLongSalida(mapaEntrada, longitudEntrada);
    if (ftruncate(descSalida, longitudSalida) < 0) {
        perror("ftruncate");
        return 1;
    }

    mapaSalida = mmap(NULL, longitudSalida, PROT_WRITE, MAP_SHARED, descSalida, 0);
    if (mapaSalida == MAP_FAILED) {
        perror("mmap Salida");
        return 1;
    }


    buffer = mmap(NULL, longitudSalida, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap buffer");
        return 1;
    }

    hijo = fork();

    if (hijo == 0) { // Proceso hijo
        signal(SIGUSR1, manejador);
        kill(getppid(), SIGUSR2);
        pause();
        procesarNumeros(buffer, 0, longitudSalida / 2);
        pause();
        procesarNumeros(buffer, longitudSalida / 2, longitudSalida);
        exit(0);
    } else {
        signal(SIGUSR2, manejador);
        pause();
        procesarMayusculasYEspacios(mapaEntrada, buffer, longitudEntrada, longitudSalida);
        wait(NULL);
    }

    memcpy(mapaSalida, buffer, longitudSalida);


    if (munmap(mapaEntrada, longitudEntrada) < 0) {
        perror("munmap Entrada");
        return 1;
    }

    if (munmap(mapaSalida, longitudSalida) < 0) {
        perror("munmap Salida");
        return 1;
    }

    if (munmap(buffer, longitudSalida) < 0) {
        perror("munmap buffer");
        return 1;
    }

    close(descEntrada);
    close(descSalida);

    return 0;
}

