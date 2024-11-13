#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]){
    char c;
    printf("PID: %d", getpid());
    scanf("%c",&c);
    return EXIT_SUCCESS;
}

