#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Vamos criar 3 filhos
    int n_filhos = 3; 

    printf("--- PAI (PID: %d) vai criar %d filhos ---\n", getpid(), n_filhos);

    for (int i = 0; i < n_filhos; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // --- ZONA EXCLUSIVA DOS FILHOS ---
            // i ajuda a identificar qual é o filho (1º, 2º ou 3º)
            printf("[FILHO %d] PID: %d. A trabalhar...\n", i+1, getpid());
            
            sleep(1); // Simular trabalho diferente
            
            // IMPORTANTE: O filho TEM de sair aqui, senão ele continua
            // no loop e cria filhos do filho!
            exit(0); 
        }
    }

    // --- ZONA EXCLUSIVA DO PAI ---
    // O Pai continua aqui e tem de esperar pelos 3 filhos
    for (int i = 0; i < n_filhos; i++) {
        wait(NULL);
    }

    printf("--- PAI: Todos os filhos terminaram. Fim. ---\n");
    return 0;
}