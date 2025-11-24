#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {


if(getenv("HOME") == NULL) {

    printf("A varivável de ambiente Home não está definida \n");

    return EXIT_FAILURE;
} else {

    printf("Está definido %s \n", getenv("HOME"));
    return EXIT_SUCCESS;
}

}