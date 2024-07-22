#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

unsigned long fin;

int cmpfunc(const void *a, const void *b) {
    return (*(unsigned long*)a - *(unsigned long*)b);
}

void clearScreen()
{
    printf("\033[H\033[2J");
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    printf("Combien de nombres premiers voulez-vous chercher ?: ");
    scanf("%ld", &fin);

    clearScreen();
    printf("recherche...");
    
    unsigned long *liste = NULL;
    liste = find(fin);
    

    if (liste == NULL)
    {
        printf("\033[0;31m");
        printf("Une erreur s'est produite\n");
        printf("\033[0;37m");
        return 1;
    }

    clearScreen();
    printf("\033[32m");
    printf("recherche terminée.\n");
    printf("\033[37m");

    clearScreen();
    printf("triage de la liste...");
    qsort(liste, fin, sizeof(unsigned long), cmpfunc);
    clearScreen();
    printf("\033[32m");
    printf("triage terminé.\n");
    printf("\033[37m");
    clearScreen();

    for (int i = 0; i < fin; i++)
    {
        printf("%ld\n", liste[i]);
    }

    return EXIT_SUCCESS;
}
