#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_LINHAS 100 // Limite máximo de threads para este exemplo

// Variáveis Globais
int total_alertas = 0;
pthread_mutex_t trinco;

// Função que cada Thread vai executar
void *registar_alerta(void *arg) {
    // 1. Bloquear o acesso (Lock)
    pthread_mutex_lock(&trinco);

    // --- Zona Crítica ---
    int temp = total_alertas;
    // Simular um pequeno atraso para forçar concorrência (opcional, mas didático)
    // usleep(100); 
    temp++;
    total_alertas = temp;
    // --------------------

    // 2. Libertar o acesso (Unlock)
    pthread_mutex_unlock(&trinco);

    return NULL;
}

int main() {
    FILE *ficheiro;
    char linha[256];
    pthread_t threads[MAX_LINHAS];
    int count = 0;

    // Inicializar o Mutex
    if (pthread_mutex_init(&trinco, NULL) != 0) {
        perror("Erro ao iniciar mutex");
        return 1;
    }

    // Abrir o ficheiro criado pelo script
    ficheiro = fopen("criticos.txt", "r");
    if (ficheiro == NULL) {
        perror("Erro ao abrir criticos.txt");
        return 1;
    }

    printf("--- A iniciar processamento de alertas ---\n");

    // Ler linha a linha e criar uma thread para cada uma
    while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
        if (count < MAX_LINHAS) {
            // Cria a thread
            if (pthread_create(&threads[count], NULL, registar_alerta, NULL) != 0) {
                perror("Erro ao criar thread");
            }
            count++;
        }
    }

    fclose(ficheiro);

    // Esperar que todas as threads terminem (Join)
    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destruir o Mutex (limpeza)
    pthread_mutex_destroy(&trinco);

    // Imprimir resultado final
    printf("Total de Alertas Processados: %d\n", total_alertas);

    return 0;
}