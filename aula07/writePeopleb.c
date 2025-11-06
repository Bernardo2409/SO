#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct
{
    int age;
    double height;
    char name[64];
} Person;

void printPersonInfo(Person *p)
{
    printf("Person: %s, %d, %f\n", p->name, p->age, p->height);
}

int main (int argc, char *argv[])
{
    /* Validate number of arguments */
    if (argc != 2)
    {
        printf("USAGE: %s fileName\n", argv[0]);
        return EXIT_FAILURE;
    }

    Person people[100];
    int numero_pessoas;

    printf("Número de pessoas? ");
    if (scanf("%d", &numero_pessoas) != 1) {
        fprintf(stderr, "Entrada inválida para número de pessoas\n");
        return EXIT_FAILURE;
    }
    if (numero_pessoas < 0 || numero_pessoas > 100) {
        fprintf(stderr, "Número de pessoas deve estar entre 0 e 100\n");
        return EXIT_FAILURE;
    }

    for (int j = 0; j < numero_pessoas; j++)
    {
        printf("Nome? ");
        if (scanf("%63s", people[j].name) != 1) {
            fprintf(stderr, "Entrada inválida para o nome\n");
            return EXIT_FAILURE;
        }

        printf("Idade? ");
        if (scanf("%d", &people[j].age) != 1) {
            fprintf(stderr, "Entrada inválida para a idade\n");
            return EXIT_FAILURE;
        }

        printf("Altura? ");
        if (scanf("%lf", &people[j].height) != 1) {
            fprintf(stderr, "Entrada inválida para a altura\n");
            return EXIT_FAILURE;
        }
    }

    /* Open the file provided as argument */
    errno = 0;
    FILE *fp = fopen(argv[1], "wb");
    if (fp == NULL)
    {
        perror("Erro ao abrir o ficheiro para escrita");
        return EXIT_FAILURE;
    }

    /* Write numero_pessoas items to the file */
    size_t written = fwrite(people, sizeof(Person), (size_t)numero_pessoas, fp);
    if (written != (size_t)numero_pessoas) {
        perror("Erro ao escrever no ficheiro");
        fclose(fp);
        return EXIT_FAILURE;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}