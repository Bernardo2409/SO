#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[])
{

    if (argc != 4) { // 1 Progrma + 3 Argumentos = 4

        printf("Insere 2 argumentos \n");

        return EXIT_FAILURE;
    } else {

    //Calculadora
    double result;
    char op = argv[2][0];

    double a = atof(argv[1]);
    double b = atof(argv[3]);
        
    switch (op)
    {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case 'x':
        result = a * b;
        break;
    case '/':
        if (b == 0.0f) {
            printf("Erro: divisão por zero\n");
            return EXIT_FAILURE;
        }
        result = a / b;
        break;
    case 'p':   
        result = pow(a,b);
        break;
    default:
        printf("Operador inválido: %c\n", op);
        return EXIT_FAILURE;
    }

    printf("Resultado: %.2f \n", result);

    return EXIT_SUCCESS;
    }
}
