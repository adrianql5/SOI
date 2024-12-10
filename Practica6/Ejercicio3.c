#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void map_file(const char *filename, int prot, int flags) {
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

    // Mapear el archivo en memoria con los permisos y flags especificados
    char *archivo_mapeado = mmap(NULL, longitud_archivo, prot, flags, fd, 0);
    if (archivo_mapeado == MAP_FAILED) {
        perror("Error al mapear el archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }
    getchar();
    // Modificar el valor de uno de los caracteres de la proyección del archivo si es de escritura
    if (prot & PROT_WRITE && longitud_archivo > 0) {
        archivo_mapeado[0] = 'X';  // Cambiar el primer carácter a 'X'
        printf("Modificado el primer carácter a 'X'\n");
    }

    // Sincronizar los cambios en memoria con el archivo si es compartido
    if (flags & MAP_SHARED) {
        if (msync(archivo_mapeado, longitud_archivo, MS_SYNC) == -1) {
            perror("Error al sincronizar los cambios en memoria con el archivo");
            munmap(archivo_mapeado, longitud_archivo);
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

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

    /*
    * El descriptor de archivos es una tabla que contiene información sobre los archivos abiertos
    * por un proceso. El identificador de archivo es un índice en la tabla de descriptores de archivos.
    */

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    getchar();
    // Mapear el archivo con diferentes configuraciones
    /*
    * Modificaciones Privadas:
    * Las modificaciones realizadas en la proyección en memoria no se reflejarán en el archivo original.
    * En su lugar se crea una copia privada de la página de memoria cuando se modifica. Por lo que los
    * cambios son visibles solo para el proceso que realizó la modificación.
    * 
    * Se suele usar cuando se necesita leer datos de un archiv y posiblemente modifcarlos, pero
    * sin que los cambios se reflejen en el archivo original ni sean visibles por otros procesos.
    * 
    * En el mapa de memoria tengo:
    * 73b7950a1000-73b7950b5000 r--p 00000000 00:1a 432050                     /home/adrianql5/Desktop/SOI/Practica6/enunciado_practica_6_2024.pdf
    * 
    * Donde 73b7950a1000 es la dirección de inicio de la proyección en memoria y 73b7950b5000 es la dirección final.
    * r--p indica que la proyección en memoria es solo de lectura y privada.
    * 00000000 indica el desplazamiento en el archivo.
    * 00:1a indica el número mayor y menor del dispositivo.
    * 432050 indica el número de i-nodo. El i-nodo es un identificador único de un archivo 
    * en un sistema de archivos.
    */

    printf("Mapeando archivo con solo lectura y área privada:\n");
    map_file(argv[1], PROT_READ, MAP_PRIVATE);

    
    /*
    * Modifcariones Compartidas:
    * Las modificaciones realizadas en la proyección en memoria se reflejarán en el archivo original.
    * Permite que multiples procesos compartan la misma proyección en memoria y los cambios realizados.
    * 
    * Se utiliza cuando se encesita que los cambios realizados en la memoria mapeada se
    * reflejen en el archivo original y sean visibles para otros procesos.
    * 
    * En el mapa de memoria tengo:
    * 
    *  73b7950a1000-73b7950b5000 rw-s 00000000 00:1a 432050                     /home/adrianql5/Desktop/SOI/Practica6/enunciado_practica_6_2024.pdf
    *
    * Donde 73b7950a1000 es la dirección de inicio de la proyección en memoria y 73b7950b5000 es la dirección final.
    * rw-s indica que la proyección en memoria es de lectura y escritura y compartida.
    * 00000000 indica el desplazamiento en el archivo.
    * 00:1a indica el número mayor y menor del dispositivo.
    * 432050 indica el número de i-nodo. El i-nodo es un identificador único de un archivo
    */

    printf("Mapeando archivo con lectura y escritura y área compartida:\n");
    map_file(argv[1], PROT_READ | PROT_WRITE, MAP_SHARED);

    return 0;
}