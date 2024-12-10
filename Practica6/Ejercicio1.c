#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/* Mapear un archivo en memoria tiene varias ventajas significativas:
*
* **Eficiencia**: Acceder a los datos del archivo a través de la memoria mapeada puede 
* ser más rápido que leerlos desde el disco cada vez que se necesitan. Esto se debe a que 
* el sistema operativo puede optimizar el acceso a los datos, cargando y descargando partes 
* del archivo según sea necesario.
*
* **Simplicidad**: Permite manipular grandes archivos como si fueran arrays en memoria, lo 
* que simplifica el código. En lugar de realizar múltiples llamadas al sistema para leer partes
* del archivo, se puede acceder directamente a los datos en memoria.
*
* **Gestión de Memoria**: El sistema operativo se encarga de gestionar la memoria, cargando en 
* memoria solo las partes del archivo que se están utilizando. Esto puede reducir el uso de memoria
*  y mejorar el rendimiento del programa.
*
* **Reducción de Copias**: Al mapear un archivo, se evita la necesidad de copiar los datos del 
* archivo a un buffer en el espacio de usuario. Esto puede reducir la sobrecarga y mejorar 
* el rendimiento.
*
* En resumen, mapear un archivo en memoria puede mejorar la eficiencia y simplicidad 
* del acceso a los datos del archivo, aprovechando las optimizaciones del sistema operativo
* para la gestión de memoria y el acceso a disco.
*/


int main(int argc, char *argv[]) {
    int fd = open (argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(1);
    }

    struct stat st;
    if(fstat(fd, &st) == -1) {
        perror("Error al obtener el tamaño del archivo");
        exit(1);
    }

    size_t longitud_archivo = st.st_size;
    printf("El tamaño del archivo es: %ld\n", longitud_archivo);

    printf("Contenido del archivo:\n");
    for(size_t i = 0; i < longitud_archivo; i++) {
        char c;
        if(read(fd, &c, 1) == -1) {
            perror("Error al leer el archivo");
            exit(1);
        }
        printf("%c", c);
    }

    printf("\n");

    char *archivo_mapeado = mmap(NULL, longitud_archivo, PROT_READ, MAP_PRIVATE, fd, 0);
    if(archivo_mapeado == MAP_FAILED) {
        perror("Error al mapear el archivo");
        close(fd);
        exit(1);
    }

    printf("Contenido de la proyección en memoria:\n");
    for(size_t i = 0; i < longitud_archivo; i++) {
        printf("%c", archivo_mapeado[i]);
    }
    printf("\n");

    if(munmap(archivo_mapeado, longitud_archivo) == -1) {
        perror("Error al liberar la proyección en memoria");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;

}
