#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 100                    // Tamanho do buffer
#define NUM_PRODUTORES 3
#define NUM_CONSUMIDORES 3
#define ITENS_POR_PRODUTOR 10

// Buffer circular
int buffer[N];
int in = 0;                      // Índice de inserção
int out = 0;                     // Índice de remoção
int total_produzido = 0;
int total_consumido = 0;

// Semáforos
sem_t mutex;                     // Exclusão mútua para buffer
sem_t vazios;                    // Conta posições vazias
sem_t cheios;                    // Conta posições cheias

void* produtor(void* num);
void* consumidor(void* num);

int main() {
    pthread_t produtores[NUM_PRODUTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];
    int ids_produtores[NUM_PRODUTORES];
    int ids_consumidores[NUM_CONSUMIDORES];
    
    // Inicializa os semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&vazios, 0, N);     // Inicialmente N posições vazias
    sem_init(&cheios, 0, 0);     // Inicialmente 0 posições cheias
    
    printf("Problema dos Produtores e Consumidores\n");
    printf("======================================\n");
    printf("Buffer: %d posições | Produtores: %d | Consumidores: %d\n", 
           N, NUM_PRODUTORES, NUM_CONSUMIDORES);
    printf("Cada produtor vai gerar %d itens\n\n", ITENS_POR_PRODUTOR);
    
    // Cria threads dos produtores
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        ids_produtores[i] = i;
        pthread_create(&produtores[i], NULL, produtor, &ids_produtores[i]);
    }
    
    // Cria threads dos consumidores
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        ids_consumidores[i] = i;
        pthread_create(&consumidores[i], NULL, consumidor, &ids_consumidores[i]);
    }
    
    // Aguarda todos os produtores terminarem
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }
    
    printf("\n>>> Todos os produtores finalizaram <<<\n\n");
    
    // Aguarda todos os consumidores terminarem
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }
    
    // Destroi os semáforos
    sem_destroy(&mutex);
    sem_destroy(&vazios);
    sem_destroy(&cheios);
    
    printf("\n======================================\n");
    printf("Programa finalizado\n");
    printf("Total produzido: %d | Total consumido: %d\n", 
           total_produzido, total_consumido);
    
    return 0;
}

void* produtor(void* num) {
    int id = *(int*)num;
    
    for (int i = 0; i < ITENS_POR_PRODUTOR; i++) {
        int item = (id * 1000) + i;  // ID único para cada item
        
        // Simula tempo de produção
        sleep(rand() % 2);
        
        // Protocolo de entrada
        sem_wait(&vazios);           // Aguarda posição vazia
        sem_wait(&mutex);            // Entra na região crítica
        
        // Seção crítica - PRODUZINDO
        buffer[in] = item;
        printf("[PRODUTOR %d] Produziu item %d na posição %d (Total no buffer: ", 
               id, item, in);
        
        in = (in + 1) % N;           // Atualiza índice circular
        total_produzido++;
        
        // Calcula itens no buffer
        int itens_buffer = (in - out + N) % N;
        if (in == out && total_produzido > total_consumido) {
            itens_buffer = N;
        }
        printf("%d)\n", itens_buffer);
        
        // Protocolo de saída
        sem_post(&mutex);            // Sai da região crítica
        sem_post(&cheios);           // Sinaliza posição cheia
    }
    
    printf("[PRODUTOR %d] Finalizou produção (%d itens)\n", id, ITENS_POR_PRODUTOR);
    pthread_exit(NULL);
}

void* consumidor(void* num) {
    int id = *(int*)num;
    int itens_consumidos = 0;
    int total_esperado = NUM_PRODUTORES * ITENS_POR_PRODUTOR;
    
    while (1) {
        // Verifica se já consumiu tudo
        if (total_consumido >= total_esperado) {
            break;
        }
        
        // Protocolo de entrada
        sem_wait(&cheios);           // Aguarda posição cheia
        sem_wait(&mutex);            // Entra na região crítica
        
        // Verifica novamente dentro da região crítica
        if (total_consumido >= total_esperado) {
            sem_post(&mutex);
            sem_post(&cheios);       // Devolve para outros consumidores
            break;
        }
        
        // Seção crítica - CONSUMINDO
        int item = buffer[out];
        printf("[CONSUMIDOR %d] Consumiu item %d da posição %d (Total no buffer: ", 
               id, item, out);
        
        out = (out + 1) % N;         // Atualiza índice circular
        total_consumido++;
        itens_consumidos++;
        
        // Calcula itens no buffer
        int itens_buffer = (in - out + N) % N;
        if (in == out) {
            itens_buffer = 0;
        }
        printf("%d)\n", itens_buffer);
        
        // Protocolo de saída
        sem_post(&mutex);            // Sai da região crítica
        sem_post(&vazios);           // Sinaliza posição vazia
        
        // Simula tempo de consumo
        sleep(rand() % 2);
    }
    
    printf("[CONSUMIDOR %d] Finalizou consumo (%d itens)\n", id, itens_consumidos);
    pthread_exit(NULL);
}