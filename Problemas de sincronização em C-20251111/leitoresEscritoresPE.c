#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_LEITORES 5
#define NUM_ESCRITORES 2

// Variáveis compartilhadas
int dados_compartilhados = 0;
int num_leitores = 0;
int num_escritores = 0;

// Semáforos
sem_t mutex_leitores;    // Protege num_leitores
sem_t mutex_escritores;  // Protege num_escritores
sem_t leitura;           // Bloqueia leitores quando há escritores esperando
sem_t escrita;           // Controla acesso exclusivo ao recurso

void* leitor(void* num);
void* escritor(void* num);

int main() {
    pthread_t leitores[NUM_LEITORES];
    pthread_t escritores[NUM_ESCRITORES];
    int ids_leitores[NUM_LEITORES];
    int ids_escritores[NUM_ESCRITORES];
    
    // Inicializa os semáforos
    sem_init(&mutex_leitores, 0, 1);
    sem_init(&mutex_escritores, 0, 1);
    sem_init(&leitura, 0, 1);
    sem_init(&escrita, 0, 1);
    
    printf("Problema dos Leitores e Escritores\n");
    printf("===================================\n");
    printf("VERSÃO: Prioridade aos ESCRITORES\n");
    printf("Leitores: %d | Escritores: %d\n\n", NUM_LEITORES, NUM_ESCRITORES);
    
    // Cria threads dos escritores
    for (int i = 0; i < NUM_ESCRITORES; i++) {
        ids_escritores[i] = i;
        pthread_create(&escritores[i], NULL, escritor, &ids_escritores[i]);
    }
    
    // Cria threads dos leitores
    for (int i = 0; i < NUM_LEITORES; i++) {
        ids_leitores[i] = i;
        pthread_create(&leitores[i], NULL, leitor, &ids_leitores[i]);
    }
    
    // Aguarda todas as threads terminarem
    for (int i = 0; i < NUM_ESCRITORES; i++) {
        pthread_join(escritores[i], NULL);
    }
    
    for (int i = 0; i < NUM_LEITORES; i++) {
        pthread_join(leitores[i], NULL);
    }
    
    // Destroi os semáforos
    sem_destroy(&mutex_leitores);
    sem_destroy(&mutex_escritores);
    sem_destroy(&leitura);
    sem_destroy(&escrita);
    
    printf("\n===================================\n");
    printf("Programa finalizado. Valor final: %d\n", dados_compartilhados);
    
    return 0;
}

void* leitor(void* num) {
    int id = *(int*)num;
    
    for (int i = 0; i < 3; i++) {
        // Protocolo de entrada
        sem_wait(&leitura);          // Bloqueia se há escritores esperando
        sem_wait(&mutex_leitores);
        num_leitores++;
        if (num_leitores == 1) {
            // Primeiro leitor bloqueia escritores
            sem_wait(&escrita);
        }
        sem_post(&mutex_leitores);
        sem_post(&leitura);
        
        // Seção crítica - LENDO
        printf("[LEITOR %d] Lendo dados... Valor: %d (Leitores ativos: %d)\n", 
               id, dados_compartilhados, num_leitores);
        sleep(1);  // Simula tempo de leitura
        
        // Protocolo de saída
        sem_wait(&mutex_leitores);
        num_leitores--;
        printf("[LEITOR %d] Terminou de ler (Leitores ativos: %d)\n", 
               id, num_leitores);
        if (num_leitores == 0) {
            // Último leitor libera escritores
            sem_post(&escrita);
        }
        sem_post(&mutex_leitores);
        
        // Faz outras coisas
        sleep(rand() % 2 + 1);
    }
    
    printf("[LEITOR %d] Finalizou suas leituras\n", id);
    pthread_exit(NULL);
}

void* escritor(void* num) {
    int id = *(int*)num;
    
    for (int i = 0; i < 2; i++) {
        // Aguarda para escrever
        sleep(rand() % 2 + 1);
        
        printf("[ESCRITOR %d] Quer escrever... Aguardando acesso\n", id);
        
        // Protocolo de entrada
        sem_wait(&mutex_escritores);
        num_escritores++;
        if (num_escritores == 1) {
            // Primeiro escritor bloqueia novos leitores
            sem_wait(&leitura);
            printf("[ESCRITOR %d] Bloqueou novos leitores\n", id);
        }
        sem_post(&mutex_escritores);
        
        // Aguarda acesso exclusivo
        sem_wait(&escrita);
        
        // Seção crítica - ESCREVENDO
        dados_compartilhados++;
        printf("[ESCRITOR %d] *** ESCREVENDO *** Novo valor: %d\n", 
               id, dados_compartilhados);
        sleep(2);  // Simula tempo de escrita
        printf("[ESCRITOR %d] Terminou de escrever\n", id);
        
        // Protocolo de saída
        sem_post(&escrita);
        
        sem_wait(&mutex_escritores);
        num_escritores--;
        if (num_escritores == 0) {
            // Último escritor libera leitores
            sem_post(&leitura);
            printf("[ESCRITOR %d] Liberou novos leitores\n", id);
        }
        sem_post(&mutex_escritores);
        
        // Faz outras coisas
        sleep(rand() % 3 + 1);
    }
    
    printf("[ESCRITOR %d] Finalizou suas escritas\n", id);
    pthread_exit(NULL);
}