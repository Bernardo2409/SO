#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* SUGESTÂO: utilize as páginas do manual para conhecer mais sobre as funções usadas:
  man opendir
  man readdir
  man stat
*/

void listDir(char dirname[])
{
    DIR *dp;
    struct dirent *dent;
    struct stat statbuf;
    char path[1024];

    dp = opendir(dirname);
    if(dp == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while((dent = readdir(dp)) != NULL)
    {
        // Ignora arquivos e diretórios ocultos (que começam com '.')
        if(dent->d_name[0] != '.')
        {
            // Monta o caminho completo
            snprintf(path, sizeof(path), "%s/%s", dirname, dent->d_name);

            // Exibe o nome do diretório ou arquivo
            printf("d  %s/\n", path);

            // Verifica se é um diretório para chamar recursivamente
            if(stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
            {
                // Se for um diretório, chama listDir recursivamente
                listDir(path);
            }
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s base_directory\n", argv[0]);
        return EXIT_FAILURE;
    }

    listDir(argv[1]);
    
    return EXIT_SUCCESS;
}
