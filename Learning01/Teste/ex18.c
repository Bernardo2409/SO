#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handle_sigint(int sig) {
    printf("Sinal SIGINT recebido!\n");
}

int main() {

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigaction(SIGINT, &sa, NULL);

    while (1) {
        printf("Esperando por SIGINT...\n");
        sleep(1);
    }
    return 0;



}