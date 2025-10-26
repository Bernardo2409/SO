#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função de comparação (ordem ascendente - ignora maiúsculas/minúsculas)
int compareAsc(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcasecmp(s1, s2);
}

// Função de comparação (ordem descendente)
int compareDesc(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcasecmp(s2, s1);
}

int main(void) {
    char input[100];
    char *words[100]; // máximo de 100 palavras
    int count = 0;

    printf("Introduza palavras (ENTER vazio para terminar):\n");

    while (1) {
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL)
            break; // EOF

        // Remover '\n' do final
        input[strcspn(input, "\n")] = '\0';

        // Linha vazia -> parar
        if (strlen(input) == 0)
            break;

        // Só aceitar se começar por letra
        if (isalpha((unsigned char)input[0])) {
            // Guardar cópia da palavra
            words[count] = strdup(input);
            if (words[count] == NULL) {
                perror("Erro de memória");
                return EXIT_FAILURE;
            }
            count++;
        }
    }

    if (count == 0) {
        printf("Nenhuma palavra válida.\n");
        return EXIT_SUCCESS;
    }

    // Ler variável de ambiente SORTORDER
    char *order = getenv("SORTORDER");

    // Ordenar as palavras (ASC por omissão)
    if (order != NULL && strcasecmp(order, "DESC") == 0) {
        qsort(words, count, sizeof(char *), compareDesc);
    } else {
        qsort(words, count, sizeof(char *), compareAsc);
    }

    // Mostrar resultado
    printf("\nPalavras ordenadas (%s):\n", (order != NULL ? order : "ASC"));
    for (int i = 0; i < count; i++) {
        printf("%s\n", words[i]);
        free(words[i]); // libertar memória alocada
    }

    return EXIT_SUCCESS;
}
