#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("Uso: %s <número1> <operador> <número2>\n", argv[0]);
        return EXIT_FAILURE;
    }

    double a, b, result;
    char op = argv[2][0];
    char *endptr;

    errno = 0;
    a = strtod(argv[1], &endptr);
    if (errno == ERANGE || endptr == argv[1] || *endptr != '\0') {
        printf("Erro: '%s' não é um número válido\n", argv[1]);
        return EXIT_FAILURE;
    }

    errno = 0;
    b = strtod(argv[3], &endptr);
    if (errno == ERANGE || endptr == argv[3] || *endptr != '\0') {
        printf("Erro: '%s' não é um número válido\n", argv[3]);
        return EXIT_FAILURE;
    }

    switch (op) {
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
            if (b == 0.0) {
                printf("Erro: Divisão por zero\n");
                return EXIT_FAILURE;
            }
            result = a / b;
            break;
        case 'p':
            result = pow(a, b);
            break;
        default:
            printf("Erro: Operador '%c' inválido\n", op);
            return EXIT_FAILURE;
    }

    printf("%.6g\n", result);
    return EXIT_SUCCESS;
}
