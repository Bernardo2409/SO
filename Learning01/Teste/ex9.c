#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

    FILE *fp = NULL;
    fp = fopen("log.txt", "r");

    if (fp == NULL) {
        printf("Erro ao abrir ficheiro!\n");
        return EXIT_FAILURE;
    }
    char line[1000];
    while(fgets(line, sizeof(line), fp) != NULL) {
        printf(" %s", line);
    }
    printf("\n");
    fclose(fp);
    return EXIT_SUCCESS;
}