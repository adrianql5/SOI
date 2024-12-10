#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void map_and_modify_file(const char *filename) {
    // Abrir el archivo para lectura y escritura
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Obtener la longitud del archivo
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Error al obtener el tamaño del archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t longitud_archivo = st.st_size;
    printf("El tamaño del archivo es: %ld\n", longitud_archivo);

    // Mapear el archivo en memoria con permisos de lectura y escritura, y área de memoria compartida
    char *archivo_mapeado = mmap(NULL, longitud_archivo, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (archivo_mapeado == MAP_FAILED) {
        perror("Error al mapear el archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Modificar el valor de uno de los caracteres de la proyección del archivo
    if (longitud_archivo > 0) {
        archivo_mapeado[0] = 'X';  // Cambiar el primer carácter a 'X'
        printf("Modificado el primer carácter a 'X'\n");
    }

    // Sincronizar los cambios en memoria con el archivo
    if (msync(archivo_mapeado, longitud_archivo, MS_SYNC) == -1) {
        perror("Error al sincronizar los cambios en memoria con el archivo");
        munmap(archivo_mapeado, longitud_archivo);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Verificar si las modificaciones se reflejan en el archivo
    printf("Contenido del archivo después de la modificación:\n");
    for (size_t i = 0; i < longitud_archivo; i++) {
        printf("%c", archivo_mapeado[i]);
    }
    printf("\n");

    // Cerrar la proyección en memoria
    if (munmap(archivo_mapeado, longitud_archivo) == -1) {
        perror("Error al liberar la proyección en memoria");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Cerrar el archivo
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    map_and_modify_file(argv[1]);

    return 0;
}