#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define NTIMES 50

static void Interrupt(int);

static volatile sig_atomic_t int_count = 0;

int main(int argc, char *argv[])
{
    struct sigaction sigact;
    unsigned int i;

    /* altera a rotina de atendimento ao ^C */
    sigact.sa_handler = Interrupt;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    if (sigaction(SIGINT, &sigact, NULL) < 0) {
        perror("Rotina de atendimento não instalada");
        return EXIT_FAILURE;
    }

    /* contador */
    printf("PID = %u\n", getpid());
    i = 0;
    while (i <= NTIMES) {
        printf("\r%08u ", i++);
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    return EXIT_SUCCESS;
}

static void Interrupt(int signum)
{
    if (signum == SIGINT) {
        int_count++;

        if (int_count < 5) {
            printf("\nCalma, ainda não cheguei a %d! (Ctrl-C %d/5)\n", NTIMES, int_count);
            fflush(stdout);
        } else if (int_count == 5) {
            /* Na 4ª interrupção, repor o comportamento por omissão para SIGINT */
            struct sigaction defact;
            defact.sa_handler = SIG_DFL;
            sigemptyset(&defact.sa_mask);
            defact.sa_flags = 0;
            if (sigaction(SIGINT, &defact, NULL) < 0) {
                perror("\nFalha ao repor a rotina por omissão para SIGINT");
                /* Mesmo que falhe, continuar a execução */
            }
            printf("\nDa próxima termino. (próximo Ctrl-C encerra o processo)\n");
            fflush(stdout);
        }
        /* Na 5ª vez, o comportamento por omissão (terminar) já estará ativo */
    } else {
        printf("Foi recebido um sinal diferente de SIGINT!\n");
        exit(EXIT_FAILURE);
    }
}