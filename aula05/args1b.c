#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i;

    if (argc != 3) { // 1 Progrma + 2 Argumentos = 3

        printf("Insere só e apenas 2 argumentos \n");

        return EXIT_FAILURE;
    } else {

    for(i = 0 ; i < argc ; i++)
    {
        printf("Argument %02d: \"%s\"\n", i, argv[i]);        
    }

    return EXIT_SUCCESS;
    }
}
