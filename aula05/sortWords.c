#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função de comparação (case-insensitive)
int compareAsc(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcasecmp(s1, s2); // ignora maiúsculas/minúsculas
}

int compareDesc(const void *a, const void *b) {
    const char *s1 = *(const char **)a;
    const char *s2 = *(const char **)b;
    return strcasecmp(s2, s1); // ordem inversa
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Uso: %s <palavras...>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Filtrar apenas palavras que começam por letra
    char *validWords[argc - 1];
    int count = 0;

    for (int i = 1; i < argc; i++) {
        if (isalpha((unsigned char)argv[i][0])) {
            validWords[count++] = argv[i];
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
        qsort(validWords, count, sizeof(char *), compareDesc);
    } else {
        qsort(validWords, count, sizeof(char *), compareAsc);
    }

    // Imprimir resultado
    printf("Palavras ordenadas (%s):\n", (order != NULL ? order : "ASC"));
    for (int i = 0; i < count; i++) {
        printf("%s\n", validWords[i]);
    }

    return EXIT_SUCCESS;
}
