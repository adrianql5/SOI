#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

pid_t hijo;
volatile sig_atomic_t signal_received = 0;

// Manejador de señales para actualizar una variable global cuando se recibe una señal
void signal_handler(int signo) {
    signal_received = 1;
}

// Calcula el nuevo tamaño del archivo de salida basado en el contenido del archivo mapeado
int calcularNuevoTamaño(const char *archivo_mapeado, size_t longitud_archivo) {
    int contador = 0;
    for (size_t i = 0; i < longitud_archivo; i++) {
        if (isdigit(archivo_mapeado[i])) {
            if(isdigit(archivo_mapeado[i+1])) {
                contador += (archivo_mapeado[i] - '0');
            }
            else contador += (archivo_mapeado[i] - '0')-1;
        } else {
            contador++;
        }
    }
    return contador;
}

// Procesa el contenido del archivo, convirtiendo mayúsculas a minúsculas y números a espacios o asteriscos
void procesarMayusculasYNumeros(char *buffer, const char *archivo_entrada, size_t inicio, size_t fin, int es_padre) {
    size_t j = inicio;
    for (size_t i = inicio; i < fin; i++) {
        if (isupper(archivo_entrada[i])) {
            buffer[j++] = tolower(archivo_entrada[i]);
        } else if (isdigit(archivo_entrada[i])) {
            if (es_padre) {
                for (int k = 0; k < archivo_entrada[i] - '0'; k++) {
                    buffer[j++] = ' ';
                }
            } else {
                for (int k = 0; k < archivo_entrada[i] - '0'; k++) {
                    buffer[j++] = '*';
                }
            }
        } else {
            buffer[j++] = archivo_entrada[i];
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <archivo entrada> <archivo salida>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Abrir archivo de entrada
    int fd_entrada = open(argv[1], O_RDONLY);
    if (fd_entrada == -1) {
        perror("Error al abrir el archivo de entrada");
        return EXIT_FAILURE;
    }

    // Obtener tamaño del archivo de entrada
    struct stat st;
    if (fstat(fd_entrada, &st) == -1) {
        perror("Error al obtener tamaño del archivo");
        close(fd_entrada);
        return EXIT_FAILURE;
    }
    size_t longitud_archivo_entrada = st.st_size;

    // Mapear archivo de entrada
    char *archivo_entrada = mmap(NULL, longitud_archivo_entrada, PROT_READ, MAP_PRIVATE, fd_entrada, 0);
    if (archivo_entrada == MAP_FAILED) {
        perror("Error al mapear archivo de entrada");
        close(fd_entrada);
        return EXIT_FAILURE;
    }

    // Calcular nuevo tamaño
    int nuevo_tamano = calcularNuevoTamaño(archivo_entrada, longitud_archivo_entrada);

    // Crear archivo de salida
    int fd_salida = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd_salida == -1) {
        perror("Error al abrir archivo de salida");
        munmap(archivo_entrada, longitud_archivo_entrada);
        close(fd_entrada);
        return EXIT_FAILURE;
    }

    // Ajustar tamaño del archivo de salida
    if (ftruncate(fd_salida, nuevo_tamano) == -1) {
        perror("Error al ajustar tamaño del archivo de salida");
        munmap(archivo_entrada, longitud_archivo_entrada);
        close(fd_entrada);
        close(fd_salida);
        return EXIT_FAILURE;
    }

    // Mapear archivo de salida
    char *archivo_salida = mmap(NULL, nuevo_tamano, PROT_WRITE, MAP_SHARED, fd_salida, 0);
    if (archivo_salida == MAP_FAILED) {
        perror("Error al mapear archivo de salida");
        munmap(archivo_entrada, longitud_archivo_entrada);
        close(fd_entrada);
        close(fd_salida);
        return EXIT_FAILURE;
    }

    // Preparar sincronización
    signal(SIGUSR1, signal_handler);

    // Crear proceso hijo
    hijo = fork();
    if (hijo == -1) {
        perror("Error al crear proceso hijo");
        munmap(archivo_entrada, longitud_archivo_entrada);
        munmap(archivo_salida, nuevo_tamano);
        close(fd_entrada);
        close(fd_salida);
        return EXIT_FAILURE;
    }

    size_t mitad = longitud_archivo_entrada / 2;

    if (hijo == 0) { // Proceso hijo
        pause();
        procesarMayusculasYNumeros(archivo_salida, archivo_entrada, 0, mitad, 0);

        kill(getppid(), SIGUSR1);
        pause();
        procesarMayusculasYNumeros(archivo_salida, archivo_entrada, mitad, longitud_archivo_entrada, 0);

        munmap(archivo_entrada, longitud_archivo_entrada);
        munmap(archivo_salida, nuevo_tamano);
        close(fd_entrada);
        close(fd_salida);
        exit(EXIT_SUCCESS);
    } else { // Proceso padre
        sleep(1);
        procesarMayusculasYNumeros(archivo_salida, archivo_entrada, 0, mitad, 1);
        kill(hijo, SIGUSR1);

        pause();
        procesarMayusculasYNumeros(archivo_salida, archivo_entrada, mitad, longitud_archivo_entrada, 1);
        kill(hijo, SIGUSR1);

        wait(NULL);
    }

    munmap(archivo_entrada, longitud_archivo_entrada);
    munmap(archivo_salida, nuevo_tamano);
    close(fd_entrada);
    close(fd_salida);
    return EXIT_SUCCESS;
}
