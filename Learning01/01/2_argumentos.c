#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // 1. Validar Argumentos
    // Lembrete: argv[0] é o nome do programa. argv[1] é o primeiro valor.
    if (argc != 2) {
        printf("Erro: Uso correto -> %s <numero>\n", argv[0]);
        return 1;
    }

    // Converter string para inteiro (ASCII to Integer)
    int valor = atoi(argv[1]);

    pid_t pid = fork();

    if (pid == 0) {
        // --- FILHO ---
        printf("[FILHO] Recebi o numero %d.\n", valor);
        int quadrado = valor * valor;
        printf("[FILHO] O quadrado é: %d\n", quadrado);
        exit(0);
    } 
    else {
        // --- PAI ---
        printf("[PAI] À espera do cálculo...\n");
        wait(NULL);
        printf("[PAI] Feito.\n");
    }

    return 0;
}