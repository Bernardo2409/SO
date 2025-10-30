#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    char text[1024];
    FILE *logFile;
    time_t currentTime;
    struct tm *timeInfo;
    char timeString[100];

    // Abrir o arquivo de log em modo "append" para adicionar novos registros ao final do arquivo
    logFile = fopen("command.log", "a");
    if (logFile == NULL) {
        fprintf(stderr, "Error: Cannot open command.log for writing\n");
        return EXIT_FAILURE;
    }

    do
    {
        printf("Command: ");
        scanf("%1023[^\n]%*c", text);

        // Verificar se o comando é "end"
        if(strcmp(text, "end") != 0) {
            // Obter o tempo atual
            time(&currentTime);
            timeInfo = localtime(&currentTime);
            strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);

            // Escrever no arquivo de log com a data e o comando
            fprintf(logFile, "[%s] %s\n", timeString, text);

            // Exibir o comando a ser executado
            printf("\n * Command to be executed: %s\n", text);
            printf("---------------------------------\n");
            system(text);  // Executar o comando
            printf("---------------------------------\n");
        }

    } while(strcmp(text, "end") != 0);

    printf("-----------The End---------------\n");

    // Fechar o arquivo de log
    fclose(logFile);

    return EXIT_SUCCESS;
}
