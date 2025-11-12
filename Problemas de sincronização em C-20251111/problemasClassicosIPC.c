/*
 * =============================================================================
 * 3 PROBLEMAS CLÁSSICOS DE SINCRONIZAÇÃO EM SISTEMAS OPERATIVOS
 * =============================================================================
 * 
 * Para compilar cada problema:
 * gcc -o problema1 problema1_produtor_consumidor.c -lpthread
 * gcc -o problema2 problema2_leitores_escritores.c -lpthread
 * gcc -o problema3 problema3_jantar_filosofos.c -lpthread
 * 
 * =============================================================================
 */

/* =============================================================================
 * PROBLEMA 1: PRODUTOR-CONSUMIDOR (40% - Mais Importante)
 * =============================================================================
 * Descrição: N produtores geram itens e M consumidores os processam através
 * de um buffer limitado (circular) de tamanho B.
 * 
 * Conceitos: Buffer limitado, sincronização bidirecional, deadlock prevention
 * Aplicações: Pipelines, filas de mensagens, buffers de I/O, thread pools
 * =============================================================================
 */

#ifdef PROBLEMA_1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 2
#define ITENS_POR_PRODUTOR 8

// Buffer circular e índices
int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int total_produzido = 0, total_consumido = 0;

// Semáforos para sincronização
sem_t mutex;      // Exclusão mútua
sem_t vazios;     // Posições vazias disponíveis
sem_t cheios;     // Posições cheias disponíveis

void* produtor(void* arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < ITENS_POR_PRODUTOR; i++) {
        int item = (id * 100) + i;
        
        sleep(rand() % 2);  // Simula tempo de produção
        
        sem_wait(&vazios);  // Aguarda vaga no buffer
        sem_wait(&mutex);   // Exclusão mútua
        
        // REGIÃO CRÍTICA - Produzir
        buffer[in] = item;
        printf("[P%d] Produziu: %d [pos=%d] Buffer: %d itens\n", 
               id, item, in, (in - out + BUFFER_SIZE) % BUFFER_SIZE + 1);
        in = (in + 1) % BUFFER_SIZE;
        total_produzido++;
        
        sem_post(&mutex);   // Libera exclusão mútua
        sem_post(&cheios);  // Sinaliza item disponível
    }
    
    printf("[P%d] *** FINALIZOU (produziu %d itens) ***\n", id, ITENS_POR_PRODUTOR);
    return NULL;
}

void* consumidor(void* arg) {
    int id = *(int*)arg;
    int consumidos = 0;
    int total_esperado = NUM_PRODUTORES * ITENS_POR_PRODUTOR;
    
    while (1) {
        if (total_consumido >= total_esperado) break;
        
        sem_wait(&cheios);  // Aguarda item no buffer
        sem_wait(&mutex);   // Exclusão mútua
        
        if (total_consumido >= total_esperado) {
            sem_post(&mutex);
            sem_post(&cheios);
            break;
        }
        
        // REGIÃO CRÍTICA - Consumir
        int item = buffer[out];
        int itens_buffer = (in - out + BUFFER_SIZE) % BUFFER_SIZE;
        if (itens_buffer == 0 && total_produzido > total_consumido) {
            itens_buffer = BUFFER_SIZE;
        }
        printf("[C%d] Consumiu: %d [pos=%d] Buffer: %d itens\n", 
               id, item, out, itens_buffer - 1);
        out = (out + 1) % BUFFER_SIZE;
        total_consumido++;
        consumidos++;
        
        sem_post(&mutex);   // Libera exclusão mútua
        sem_post(&vazios);  // Sinaliza vaga disponível
        
        sleep(rand() % 2);  // Simula tempo de consumo
    }
    
    printf("[C%d] *** FINALIZOU (consumiu %d itens) ***\n", id, consumidos);
    return NULL;
}

int main() {
    pthread_t prods[NUM_PRODUTORES], cons[NUM_CONSUMIDORES];
    int ids_p[NUM_PRODUTORES], ids_c[NUM_CONSUMIDORES];
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║   PROBLEMA 1: PRODUTOR-CONSUMIDOR (40%%)      ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("Buffer: %d | Produtores: %d | Consumidores: %d\n\n",
           BUFFER_SIZE, NUM_PRODUTORES, NUM_CONSUMIDORES);
    
    sem_init(&mutex, 0, 1);
    sem_init(&vazios, 0, BUFFER_SIZE);
    sem_init(&cheios, 0, 0);
    
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        ids_p[i] = i;
        pthread_create(&prods[i], NULL, produtor, &ids_p[i]);
    }
    
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        ids_c[i] = i;
        pthread_create(&cons[i], NULL, consumidor, &ids_c[i]);
    }
    
    for (int i = 0; i < NUM_PRODUTORES; i++)
        pthread_join(prods[i], NULL);
    for (int i = 0; i < NUM_CONSUMIDORES; i++)
        pthread_join(cons[i], NULL);
    
    sem_destroy(&mutex);
    sem_destroy(&vazios);
    sem_destroy(&cheios);
    
    printf("\n✓ Total: Produzido=%d | Consumido=%d\n\n", 
           total_produzido, total_consumido);
    return 0;
}

#endif

/* =============================================================================
 * PROBLEMA 2: LEITORES-ESCRITORES (35%)
 * =============================================================================
 * Descrição: Múltiplos leitores podem ler simultaneamente, mas escritores
 * precisam de acesso exclusivo. Esta versão dá prioridade aos ESCRITORES.
 * 
 * Conceitos: Acesso compartilhado vs exclusivo, starvation, prioridades
 * Aplicações: Databases, caches, sistemas de arquivos, estruturas compartilhadas
 * =============================================================================
 */

#ifdef PROBLEMA_2

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_LEITORES 4
#define NUM_ESCRITORES 2
#define OPERACOES_LEITOR 3
#define OPERACOES_ESCRITOR 2

int dados = 0;              // Recurso compartilhado
int num_leitores = 0;       // Leitores ativos
int num_escritores = 0;     // Escritores esperando/ativos

sem_t mutex_r;              // Protege num_leitores
sem_t mutex_w;              // Protege num_escritores
sem_t leitura;              // Bloqueia leitores quando há escritores
sem_t escrita;              // Acesso exclusivo ao recurso

void* leitor(void* arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < OPERACOES_LEITOR; i++) {
        sleep(rand() % 2);
        
        sem_wait(&leitura);       // Bloqueado se há escritores
        sem_wait(&mutex_r);
        num_leitores++;
        if (num_leitores == 1)
            sem_wait(&escrita);   // Primeiro leitor bloqueia escrita
        sem_post(&mutex_r);
        sem_post(&leitura);
        
        // LENDO
        printf("  [L%d] Lendo: %d (leitores=%d)\n", id, dados, num_leitores);
        sleep(1);
        
        sem_wait(&mutex_r);
        num_leitores--;
        printf("  [L%d] Terminou leitura (leitores=%d)\n", id, num_leitores);
        if (num_leitores == 0)
            sem_post(&escrita);   // Último leitor libera escrita
        sem_post(&mutex_r);
    }
    
    printf("  [L%d] ✓ Finalizou\n", id);
    return NULL;
}

void* escritor(void* arg) {
    int id = *(int*)arg;
    
    for (int i = 0; i < OPERACOES_ESCRITOR; i++) {
        sleep(rand() % 3);
        
        printf("[E%d] >>> Quer escrever...\n", id);
        
        sem_wait(&mutex_w);
        num_escritores++;
        if (num_escritores == 1)
            sem_wait(&leitura);   // Primeiro escritor bloqueia leitores
        sem_post(&mutex_w);
        
        sem_wait(&escrita);       // Aguarda acesso exclusivo
        
        // ESCREVENDO
        dados += 10;
        printf("[E%d] *** ESCREVENDO *** Novo valor: %d\n", id, dados);
        sleep(2);
        
        sem_post(&escrita);
        
        sem_wait(&mutex_w);
        num_escritores--;
        if (num_escritores == 0)
            sem_post(&leitura);   // Último escritor libera leitores
        sem_post(&mutex_w);
        
        printf("[E%d] <<< Terminou escrita\n", id);
    }
    
    printf("[E%d] ✓ Finalizou\n", id);
    return NULL;
}

int main() {
    pthread_t leitores[NUM_LEITORES], escritores[NUM_ESCRITORES];
    int ids_l[NUM_LEITORES], ids_e[NUM_ESCRITORES];
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║  PROBLEMA 2: LEITORES-ESCRITORES (35%%)       ║\n");
    printf("║  Versão: Prioridade aos ESCRITORES           ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("Leitores: %d | Escritores: %d\n\n", NUM_LEITORES, NUM_ESCRITORES);
    
    sem_init(&mutex_r, 0, 1);
    sem_init(&mutex_w, 0, 1);
    sem_init(&leitura, 0, 1);
    sem_init(&escrita, 0, 1);
    
    for (int i = 0; i < NUM_ESCRITORES; i++) {
        ids_e[i] = i;
        pthread_create(&escritores[i], NULL, escritor, &ids_e[i]);
    }
    
    for (int i = 0; i < NUM_LEITORES; i++) {
        ids_l[i] = i;
        pthread_create(&leitores[i], NULL, leitor, &ids_l[i]);
    }
    
    for (int i = 0; i < NUM_ESCRITORES; i++)
        pthread_join(escritores[i], NULL);
    for (int i = 0; i < NUM_LEITORES; i++)
        pthread_join(leitores[i], NULL);
    
    sem_destroy(&mutex_r);
    sem_destroy(&mutex_w);
    sem_destroy(&leitura);
    sem_destroy(&escrita);
    
    printf("\n✓ Valor final: %d\n\n", dados);
    return 0;
}

#endif

/* =============================================================================
 * PROBLEMA 3: JANTAR DOS FILÓSOFOS (25%)
 * =============================================================================
 * Descrição: 5 filósofos sentados em mesa circular. Cada um precisa de 2
 * garfos (esquerdo e direito) para comer. Deve evitar deadlock.
 * 
 * Conceitos: Deadlock prevention, resource ordering, starvation
 * Aplicações: Alocação de recursos, transações distribuídas, scheduling
 * =============================================================================
 */

#ifdef PROBLEMA_3

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define PENSANDO 0
#define COM_FOME 1
#define COMENDO 2
#define ESQ(i) ((i + N - 1) % N)
#define DIR(i) ((i + 1) % N)

int estado[N];
sem_t mutex;
sem_t s[N];

void testar(int i) {
    if (estado[i] == COM_FOME && 
        estado[ESQ(i)] != COMENDO && 
        estado[DIR(i)] != COMENDO) {
        
        estado[i] = COMENDO;
        printf("    [F%d] 🍝 COMENDO (garfos %d e %d)\n", i, ESQ(i), i);
        sem_post(&s[i]);
    }
}

void pegar_garfos(int i) {
    sem_wait(&mutex);
    estado[i] = COM_FOME;
    printf("    [F%d] 😋 COM FOME\n", i);
    testar(i);
    sem_post(&mutex);
    sem_wait(&s[i]);
}

void devolver_garfos(int i) {
    sem_wait(&mutex);
    estado[i] = PENSANDO;
    printf("    [F%d] 🤔 PENSANDO (devolveu garfos)\n", i);
    testar(ESQ(i));
    testar(DIR(i));
    sem_post(&mutex);
}

void* filosofo(void* arg) {
    int i = *(int*)arg;
    
    for (int j = 0; j < 3; j++) {
        // PENSAR
        printf("    [F%d] 💭 Pensando...\n", i);
        sleep(rand() % 3 + 1);
        
        // PEGAR GARFOS
        pegar_garfos(i);
        
        // COMER
        sleep(rand() % 2 + 1);
        printf("    [F%d] ✓ Terminou de comer (ciclo %d/3)\n", i, j + 1);
        
        // DEVOLVER GARFOS
        devolver_garfos(i);
    }
    
    printf("    [F%d] ═══ SAIU DA MESA ═══\n", i);
    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N];
    
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║    PROBLEMA 3: JANTAR DOS FILÓSOFOS (25%%)    ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("5 filósofos, 5 garfos, sem deadlock\n\n");
    
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++) {
        sem_init(&s[i], 0, 0);
        estado[i] = PENSANDO;
        ids[i] = i;
    }
    
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, filosofo, &ids[i]);
        printf("[F%d] Sentou-se à mesa\n", i);
    }
    
    printf("\n--- JANTAR INICIADO ---\n\n");
    
    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);
    
    sem_destroy(&mutex);
    for (int i = 0; i < N; i++)
        sem_destroy(&s[i]);
    
    printf("\n✓ Todos os filósofos terminaram\n\n");
    return 0;
}

#endif

/* =============================================================================
 * INSTRUÇÕES DE COMPILAÇÃO E EXECUÇÃO
 * =============================================================================
 * 
 * Para compilar PROBLEMA 1 (Produtor-Consumidor):
 *   gcc -DPROBLEMA_1 -o problema1 este_arquivo.c -lpthread
 *   ./problema1
 * 
 * Para compilar PROBLEMA 2 (Leitores-Escritores):
 *   gcc -DPROBLEMA_2 -o problema2 este_arquivo.c -lpthread
 *   ./problema2
 * 
 * Para compilar PROBLEMA 3 (Jantar dos Filósofos):
 *   gcc -DPROBLEMA_3 -o problema3 este_arquivo.c -lpthread
 *   ./problema3
 * 
 * =============================================================================
 * COMPARAÇÃO DOS 3 PROBLEMAS
 * =============================================================================
 * 
 * ┌─────────────────────┬──────────┬────────────┬─────────────────────────┐
 * │ Problema            │ Ranking  │ Semáforos  │ Aplicação Principal     │
 * ├─────────────────────┼──────────┼────────────┼─────────────────────────┤
 * │ Produtor-Consumidor │ 40% (1º) │ 3 (m,v,c)  │ Pipelines, Filas        │
 * │ Leitores-Escritores │ 35% (2º) │ 4 (mr,mw,  │ Databases, Caches       │
 * │                     │          │    l,e)    │                         │
 * │ Jantar Filósofos    │ 25% (3º) │ 6 (m,s[5]) │ Deadlock Prevention     │
 * └─────────────────────┴──────────┴────────────┴─────────────────────────┘
 * 
 * =============================================================================
 */