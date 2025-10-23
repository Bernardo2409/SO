#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Uso: %s <palavras...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int totalLength = 0;

    for (int i = 1; i < argc; i++) {
        if (isalpha((unsigned char)argv[i][0])) {
            totalLength += strlen(argv[i]) + 1; 
        }
    }

    if (totalLength == 0) {
        printf("Nenhum argumento válido.\n");
        return EXIT_SUCCESS;
    }

    char *sentence = malloc(totalLength * sizeof(char));
    if (sentence == NULL) {
        perror("Erro a alocar memória");
        return EXIT_FAILURE;
    }

    sentence[0] = '\0'; 

    for (int i = 1; i < argc; i++) {
        if (isalpha((unsigned char)argv[i][0])) {
            strcat(sentence, argv[i]);
            strcat(sentence, " ");
        }
    }

    if (strlen(sentence) > 0 && sentence[strlen(sentence) - 1] == ' ')
        sentence[strlen(sentence) - 1] = '\0';

    printf("%s\n", sentence);

    free(sentence);
    return EXIT_SUCCESS;
}
