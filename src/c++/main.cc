#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <string.h>
#ifdef BENCH
#include <iostream>
#include <chrono>
#endif

unsigned int *find(unsigned int const& fin);

int main(int argc, char *argv[]) {
    #ifndef BENCH
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    unsigned int fin;
    bool preDefinedEnd = argc > 1 ? true : false;
    if (preDefinedEnd)
    {
        sscanf(argv[1], "%d", &fin);
    }
    else
    {
        printf("Combien de nombres premiers voulez-vous chercher ?: ");
        scanf("%d", &fin);
    }

    printf("recherche...");
    unsigned int *liste = find(fin);

    if (!liste)
    {
        printf("\033[0;31m");
        printf("Une erreur s'est produite\n");
        printf("\033[0;37m");
        return 1;
    }

    printf("\033[32m");
    printf("recherche terminée.\n");
    printf("\033[37m");

    for (unsigned int i = 0; i < fin; i++)
    {
        printf("%d\n", liste[i]);
    }

    free(liste);
    #else
    unsigned int npf;
    printf("Combien de nombres premiers voulez-vous chercher ?\n");
    scanf("%d", &npf);
    const auto start = std::chrono::high_resolution_clock::now();
    unsigned int *liste = find(npf);
    const auto stop = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds{stop - start};
    std::cout << "Temps : " << elapsed_seconds.count() << "  secondes" << std::endl;
    #endif
    
    return EXIT_SUCCESS;
}
