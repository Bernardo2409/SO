#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Uso: %s <limite_inferior> <limite_superior>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int min = atoi(argv[1]);
    int max = atoi(argv[2]);

    if (min >= max) {
        printf("Erro: o limite inferior deve ser menor que o superior.\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    int secreto = rand() % (max - min + 1) + min;
    int tentativa, tentativas = 0;

    printf("Adivinhe o número entre %d e %d!\n", min, max);

    do {
        printf("Introduza o seu palpite: ");
        if (scanf("%d", &tentativa) != 1) {
            printf("Entrada inválida. Introduza um número inteiro.\n");
            while (getchar() != '\n'); 
            continue;
        }

        tentativas++;

        if (tentativa < secreto)
            printf("Mais alto!\n");
        else if (tentativa > secreto)
            printf("Mais baixo!\n");
        else
            printf("Acertou em %d tentativas!\n", tentativas);

    } while (tentativa != secreto);

    return EXIT_SUCCESS;
}
