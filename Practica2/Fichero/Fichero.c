#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    // Abrir un fichero antes del fork
    int fd = open("archivo_compartido.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al abrir el fichero");
        exit(1);
    }

    // Escribir en el fichero desde el padre antes de fork
    const char *mensaje_inicial = "Escrito antes de fork\n";
    write(fd, mensaje_inicial, sizeof(char) * 22); // 22 = longitud del mensaje

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error en fork");
        close(fd);
        exit(1);
    } else if (pid == 0) {
        // Proceso hijo
        const char *mensaje_hijo = "Escrito desde el hijo\n";
        write(fd, mensaje_hijo, sizeof(char) * 22);

        // Leer desde el hijo (desplazar puntero al inicio del fichero)
        lseek(fd, 0, SEEK_SET);
        char buffer[256];
        ssize_t leidos = read(fd, buffer, sizeof(buffer) - 1);
        if (leidos >= 0) {
            buffer[leidos] = '\0';
            printf("[Hijo] Contenido del fichero:\n%s", buffer);
        } else {
            perror("[Hijo] Error al leer el fichero");
        }
        close(fd);
    } else {
        // Proceso padre
        wait(NULL); // Esperar al hijo

        const char *mensaje_padre = "Escrito desde el padre\n";
        write(fd, mensaje_padre, sizeof(char) * 23);

        // Leer desde el padre (desplazar puntero al inicio del fichero)
        lseek(fd, 0, SEEK_SET);
        char buffer[256];
        ssize_t leidos = read(fd, buffer, sizeof(buffer) - 1);
        if (leidos >= 0) {
            buffer[leidos] = '\0';
            printf("[Padre] Contenido del fichero:\n%s", buffer);
        } else {
            perror("[Padre] Error al leer el fichero");
        }
        close(fd);
    }

    return 0;
}

