#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int a_flag = 0;
    int b_flag = 0;
    int c_flag = 0;

    

    for(int i = 1; i < argc; i++) {

        switch (argv[i][1])
        {
        case 'a':
            a_flag = 1;
            break;
        case 'b':
            b_flag = 1;
            break;
        case 'c':
            c_flag = 1;
            break;
        }
    } 
    return EXIT_SUCCESS;

}