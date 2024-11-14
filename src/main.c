#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <string.h>

unsigned int *find(unsigned int fin);

int main(int argc, char *argv[]) {
    #ifndef DEBUG
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    unsigned int fin;
    bool preDefinedEnd = argc > 1 ? true : false;
    bool debug = preDefinedEnd && strcmp(argv[2], "true") == 0 ? true : false;
    if (preDefinedEnd)
    {
        sscanf(argv[1], "%d", &fin);
    }
    else
    {
        printf("Combien de nombres premiers voulez-vous chercher ?: ");
        scanf("%d", &fin);
    }

    if(!debug)
    {
        printf("recherche...");
    }
    unsigned int *liste = find(fin);

    if (!liste)
    {
        printf("\033[0;31m");
        printf("Une erreur s'est produite\n");
        printf("\033[0;37m");
        return 1;
    }

    if(!debug)
    {
        printf("\033[32m");
        printf("recherche terminée.\n");
        printf("\033[37m");

        for (unsigned int i = 0; i < fin; i++)
        {
            printf("%d\n", liste[i]);
        }
    }
    #else
    unsigned int *liste = find(1000000);
    #endif
    
    return EXIT_SUCCESS;
}
