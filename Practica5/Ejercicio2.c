#include <stdio.h>
#include <unistd.h>

/*
 * Código escrito por
 *
 * Izan Monterroso
 * Adrián Quiroga
 *
 * Las funciones van al segmento de texto.
 *
 * Los valores se pasan por referencia a las funciones
 * y se genera una copia. Podemos ver que ambos argumentos
 * y variables ocupan las mismas direcciones de memoria.
 * Esto es porque al acabar su trabajo, la primera funcion
 * libera el espacio que ocupa y es ocupado por la segunda
 * funcion. Coinciden exactamente porque tienen los mismos
 * tipos de datos en la misma cantidad.
 *
 */

void funcion1(int arg){
    int c = 2;
    printf("Dir mem arg funcion1: %p\n", (void*)&arg);
    printf("Dir mem vLocal funcion1: %p\n", (void*)&c);
}

void funcion2(int argu){
    int d = 9;
    printf("Dir mem arg funcion2: %p\n", (void*)&argu);
    printf("Dir mem vLocal funcion2: %p\n", (void*)&d);
}

int main(){
    int a = 1, b = 8;
    printf("Pid: %d\n", getpid());

    funcion1(a);
    funcion2(b);

    void (*ptr_func1)(int)=funcion1;
    void (*ptr_func2)(int)=funcion2;

    printf("Dir meme funcion1: %p\n",ptr_func1);
    printf("Dir meme funcion2: %p\n",ptr_func2);

    scanf("%d", &a);

    return 0;
}

