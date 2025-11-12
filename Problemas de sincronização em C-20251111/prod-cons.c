#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full;           // Contadores de espaço livre e itens cheios
pthread_mutex_t mutex;       // Exclusão mútua

void* produtor(void* arg) {
    for (int i = 0; i < 10; i++) {
        int item = rand() % 100;

        sem_wait(&empty);            // Espera espaço livre
        pthread_mutex_lock(&mutex);  // Entra na seção crítica

        buffer[in] = item;
        printf("[Produtor] Inseriu %d na posição %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);             // Sinaliza item disponível

        sleep(1);
    }
    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&full);             // Espera item disponível
        pthread_mutex_lock(&mutex);  // Entra na seção crítica

        int item = buffer[out];
        printf("[Consumidor] Retirou %d da posição %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);            // Sinaliza espaço livre

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t tProd, tCons;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tProd, NULL, produtor, NULL);
    pthread_create(&tCons, NULL, consumidor, NULL);

    pthread_join(tProd, NULL);
    pthread_join(tCons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
