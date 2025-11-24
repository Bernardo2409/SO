#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (fork() == 0) {

        printf("PAI: %d \n", getppid());
        printf("FILHO: %d \n", getpid());

    }
    return EXIT_SUCCESS;
    

}