#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Uso: %s <palavras...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int totalLength = 0;
    for (int i = 1; i < argc; i++) {
        totalLength += strlen(argv[i]) + 1; // +1 para o espaço ou '\0'
    }

    char *sentence = malloc(totalLength * sizeof(char));
    if (sentence == NULL) {
        perror("Erro a alocar memória");
        return EXIT_FAILURE;
    }

    sentence[0] = '\0'; 
    for (int i = 1; i < argc; i++) {
        strcat(sentence, argv[i]);
        if (i < argc - 1)
            strcat(sentence, " "); 
    }

    printf("%s\n", sentence);

    free(sentence);
    return EXIT_SUCCESS;
}
