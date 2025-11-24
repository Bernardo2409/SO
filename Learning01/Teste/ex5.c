#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    FILE *fp = NULL;
    fp = fopen("dados.txt", "r");

    if (fp == NULL) {

        printf("O ficheiro não existe \n");
        return EXIT_FAILURE;
    } else {

        printf("O ficheiro foi aberto com sucesso \n");
        fclose(fp);
        
        return EXIT_SUCCESS;
    }

}