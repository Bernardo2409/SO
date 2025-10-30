#include <stdio.h>
#include <stdlib.h>

int compareInts(const void *px1, const void *px2)
{
    int x1 = *((const int *)px1);
    int x2 = *((const int *)px2);
    return (x1 < x2 ? -1 : x1 == x2 ? 0 : 1);
}

int main(int argc, char *argv[])
{
    FILE *file;
    int *numbers = NULL;
    int numSize = 0;
    int capacity = 10; // Capacidade inicial do array
    int i;

    // Verificar se foi fornecido o nome do arquivo
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Abrir o arquivo
    file = fopen(argv[1], "r");
    if(file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Alocar memória inicial para os números
    numbers = malloc(sizeof(int) * capacity);
    if(numbers == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Ler números do arquivo e redimensionar o array conforme necessário
    int number;
    while (fscanf(file, "%d", &number) == 1) {
        // Garantir espaço no array, redimensionando se necessário
        if (numSize >= capacity) {
            capacity *= 2; // Dobra a capacidade
            int *tmp = realloc(numbers, sizeof(int) * capacity);
            if (tmp == NULL) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                free(numbers);
                fclose(file);
                return EXIT_FAILURE;
            }
            numbers = tmp;
        }
        numbers[numSize++] = number;
    }

    // Fechar o arquivo
    fclose(file);

    if (numSize == 0) {
        printf("No integers found in file %s\n", argv[1]);
        free(numbers);
        return EXIT_SUCCESS;
    }

    // Ordenar os números usando qsort
    if (numSize > 1) {
        qsort(numbers, numSize, sizeof(int), compareInts);
    }

    // Imprimir os números ordenados
    printf("Sorted numbers from file %s:\n", argv[1]);
    for(i = 0; i < numSize; i++) {
        printf("%d\n", numbers[i]);
    }

    // Liberar memória
    free(numbers);

    return EXIT_SUCCESS;
}
