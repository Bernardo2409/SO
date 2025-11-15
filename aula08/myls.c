#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("================================================\n");
    sleep(1);
    system("ls -la");
    sleep(1);
    printf("================================================\n");


    return EXIT_SUCCESS;
}
