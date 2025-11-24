#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("--- Início do Processo Original (PID: %d) ---\n", getpid());

    // 1. Criar o processo
    pid_t pid = fork(); // valor de retorno dos processos, pid = 0 para processos filhos e pid != getpid()

    // 2. Verificar erros
    if (pid < 0) {
        perror("Erro no fork");
        return 1;
    }

    // 3. Lógica separada
    if (pid == 0) {
        // --- CÓDIGO DO FILHO ---
        // O filho "acorda" aqui
        printf("[FILHO] Sou o novo processo! Meu PID: %d, Pai: %d\n", getpid(), getppid());
        sleep(2); // Simula trabalho
        printf("[FILHO] Terminei.\n");
        exit(0); // OBRIGATÓRIO: O filho deve sair explicitamente!
    } 
    else {
        // --- CÓDIGO DO PAI ---
        // O pai recebe o PID do filho (ex: 1234)
        printf("[PAI] Criei um filho com PID %d.\n", pid);
        
        // Espera que o filho morra
        wait(NULL); 
        printf("[PAI] O meu filho já terminou. Vou sair.\n");
    }

    return 0;
}
