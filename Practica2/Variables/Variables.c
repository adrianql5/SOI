#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global_var = 10; // Variable global

int main() {
    int local_var = 20; // Variable local
    int *dynamic_var = malloc(sizeof(int)); // Variable dinámica
    *dynamic_var = 30;

    printf("Antes de fork:\n");
    printf("  Dirección de variable global: %p, valor: %d\n", (void*)&global_var, global_var);
    printf("  Dirección de variable local: %p, valor: %d\n", (void*)&local_var, local_var);
    printf("  Dirección de variable dinámica: %p, valor: %d\n", (void*)dynamic_var, *dynamic_var);

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error en fork");
        free(dynamic_var);
        exit(1);
    } else if (pid == 0) {
        // Proceso hijo
        printf("\n[Hijo] Después de fork:\n");
        printf("  Dirección de variable global: %p, valor: %d\n", (void*)&global_var, global_var);
        printf("  Dirección de variable local: %p, valor: %d\n", (void*)&local_var, local_var);
        printf("  Dirección de variable dinámica: %p, valor: %d\n", (void*)dynamic_var, *dynamic_var);

        // Modificar valores
        global_var = 100;
        local_var = 200;
        *dynamic_var = 300;

        printf("\n[Hijo] Después de modificar:\n");
        printf("  Dirección de variable global: %p, valor: %d\n", (void*)&global_var, global_var);
        printf("  Dirección de variable local: %p, valor: %d\n", (void*)&local_var, local_var);
        printf("  Dirección de variable dinámica: %p, valor: %d\n", (void*)dynamic_var, *dynamic_var);
    } else {
        // Proceso padre
        wait(NULL); // Espera al hijo
        printf("\n[Padre] Después de fork (antes de cualquier modificación):\n");
        printf("  Dirección de variable global: %p, valor: %d\n", (void*)&global_var, global_var);
        printf("  Dirección de variable local: %p, valor: %d\n", (void*)&local_var, local_var);
        printf("  Dirección de variable dinámica: %p, valor: %d\n", (void*)dynamic_var, *dynamic_var);
    }

    free(dynamic_var); // Liberar memoria dinámica
    return 0;
}

