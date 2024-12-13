#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>

void manejador(int señal) {
    // Manejador de señales vacío, solo para sincronización
}

char* procesarMayusculasYHuecos(const char *archivo_entrada, int inicio, int final) {
    int j = 0;
    char *buffer = (char*)malloc((final - inicio + 1) * sizeof(char));
    if (!buffer) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    for (int i = inicio; i < final; i++) {
        if (isupper(archivo_entrada[i])) {
            buffer[j++] = archivo_entrada[i];
        } else if (isdigit(archivo_entrada[i])) {
            int repeticiones = archivo_entrada[i] - '0';
            for (int k = 0; k < repeticiones; k++) {
                buffer = realloc(buffer, (j + 1) * sizeof(char));
                if (!buffer) {
                    perror("Error al reasignar memoria");
                    exit(EXIT_FAILURE);
                }
                buffer[j++] = '9';
            }
        } else {
            buffer[j++] = archivo_entrada[i];
        }
    }
    buffer[j] = '\0'; // Agregar terminador de cadena
    return buffer;
}

void cambiarNuevesPorAsteriscos(char *buffer) {
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == '9') {
            buffer[i] = '*';
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo entrada> <archivo salida>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo de entrada");
        return EXIT_FAILURE;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error al obtener el tamaño del archivo");
        close(fd);
        return EXIT_FAILURE;
    }

    int longitud_archivo_entrada = st.st_size;

    char *archivo_entrada = mmap(NULL, longitud_archivo_entrada, PROT_READ, MAP_PRIVATE, fd, 0);
    if (archivo_entrada == MAP_FAILED) {
        perror("Error al mapear el archivo de entrada");
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);

    int fd_salida = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd_salida == -1) {
        perror("Error al abrir el archivo de salida");
        munmap(archivo_entrada, longitud_archivo_entrada);
        return EXIT_FAILURE;
    }

    ftruncate(fd_salida, longitud_archivo_entrada);

    char *archivo_salida = mmap(NULL, longitud_archivo_entrada, PROT_READ | PROT_WRITE, MAP_SHARED, fd_salida, 0);
    if (archivo_salida == MAP_FAILED) {
        perror("Error al mapear el archivo de salida");
        close(fd_salida);
        munmap(archivo_entrada, longitud_archivo_entrada);
        return EXIT_FAILURE;
    }

    signal(SIGUSR1, manejador);

    pid_t hijo = fork();
    if (hijo == -1) {
        perror("Error al crear el proceso hijo");
        munmap(archivo_entrada, longitud_archivo_entrada);
        munmap(archivo_salida, longitud_archivo_entrada);
        close(fd_salida);
        return EXIT_FAILURE;
    }

    size_t mitad = longitud_archivo_entrada / 2;

    if (hijo == 0) { // Proceso hijo
        kill(getppid(), SIGUSR1);
        pause();

        cambiarNuevesPorAsteriscos(archivo_salida);

        kill(getppid(), SIGUSR1);
        exit(EXIT_SUCCESS);
    } else { // Proceso padre
        pause();
        char *buffer = procesarMayusculasYHuecos(archivo_entrada, 0, mitad);

        memcpy(archivo_salida, buffer, strlen(buffer));
        size_t tam1 = strlen(buffer);
        free(buffer);

        kill(hijo, SIGUSR1);
        pause();

        char *buffer2 = procesarMayusculasYHuecos(archivo_entrada, mitad, longitud_archivo_entrada);

        memcpy(archivo_salida + tam1, buffer2, strlen(buffer2));
        free(buffer2);

        wait(NULL);

        munmap(archivo_entrada, longitud_archivo_entrada);
        munmap(archivo_salida, longitud_archivo_entrada);
        close(fd_salida);
    }

    return EXIT_SUCCESS;
}

