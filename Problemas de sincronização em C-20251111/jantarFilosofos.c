#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5                    // Número de filósofos
#define PENSANDO 0
#define COM_FOME 1
#define COMENDO 2
#define ESQUERDA (i + N - 1) % N   // Filósofo à esquerda
#define DIREITA (i + 1) % N        // Filósofo à direita

int estado[N];                 // Estado de cada filósofo
sem_t mutex;                   // Semáforo para exclusão mútua
sem_t s[N];                    // Semáforo para cada filósofo

void* filosofo(void* num);
void pegar_garfos(int i);
void devolver_garfos(int i);
void testar(int i);
void pensar(int i);
void comer(int i);

int main() {
    pthread_t thread_id[N];
    int filosofo_num[N];
    
    // Inicializa o semáforo mutex
    sem_init(&mutex, 0, 1);
    
    // Inicializa os semáforos dos filósofos
    for (int i = 0; i < N; i++) {
        sem_init(&s[i], 0, 0);
        estado[i] = PENSANDO;
        filosofo_num[i] = i;
    }
    
    printf("Problema dos Filósofos Jantares\n");
    printf("================================\n\n");
    
    // Cria as threads dos filósofos
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, filosofo, &filosofo_num[i]);
        printf("Filósofo %d sentou-se à mesa\n", i);
    }
    
    // Aguarda as threads terminarem
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }
    
    // Destroi os semáforos
    sem_destroy(&mutex);
    for (int i = 0; i < N; i++) {
        sem_destroy(&s[i]);
    }
    
    return 0;
}

void* filosofo(void* num) {
    int i = *(int*)num;
    
    // Simula algumas refeições
    for (int j = 0; j < 3; j++) {
        pensar(i);
        pegar_garfos(i);
        comer(i);
        devolver_garfos(i);
    }
    
    printf("Filósofo %d terminou de jantar e saiu\n", i);
    pthread_exit(NULL);
}

void pegar_garfos(int i) {
    sem_wait(&mutex);              // Entra na região crítica
    estado[i] = COM_FOME;
    printf("Filósofo %d está COM FOME\n", i);
    testar(i);                     // Tenta pegar os garfos
    sem_post(&mutex);              // Sai da região crítica
    sem_wait(&s[i]);               // Bloqueia se não conseguiu pegar os garfos
}

void devolver_garfos(int i) {
    sem_wait(&mutex);              // Entra na região crítica
    estado[i] = PENSANDO;
    printf("Filósofo %d devolveu os garfos (esquerdo e direito)\n", i);
    
    // Testa se os vizinhos podem comer agora
    testar(ESQUERDA);
    testar(DIREITA);
    
    sem_post(&mutex);              // Sai da região crítica
}

void testar(int i) {
    // Se o filósofo está com fome e seus vizinhos não estão comendo
    if (estado[i] == COM_FOME && 
        estado[ESQUERDA] != COMENDO && 
        estado[DIREITA] != COMENDO) {
        
        estado[i] = COMENDO;
        printf("Filósofo %d pegou os garfos %d e %d (está COMENDO)\n", 
               i, ESQUERDA, i);
        sem_post(&s[i]);           // Sinaliza que pode comer
    }
}

void pensar(int i) {
    printf("Filósofo %d está PENSANDO\n", i);
    sleep(rand() % 3 + 1);         // Pensa por 1-3 segundos
}

void comer(int i) {
    sleep(rand() % 3 + 1);         // Come por 1-3 segundos
    printf("Filósofo %d terminou de COMER\n", i);
}