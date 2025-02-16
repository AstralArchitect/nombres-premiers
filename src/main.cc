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
unsigned int* find_gpu(unsigned int const& fin);

int main(int argc, char *argv[]) {
    #ifndef BENCH
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    unsigned int fin;
    bool preDefinedEnd = argc > 1 ? true : false;
    // verify if the user wants to force use the gpu
    bool force_gpu = false;
    if (strcmp(argv[2], "--force-gpu") == 0)
    {
        force_gpu = true;
    }
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
    #ifndef CUDA
    unsigned int *liste = find(fin);
    #else
    unsigned int *liste;
    if (fin < 500000 || !force_gpu)
        liste = find(fin);
    else
        liste = find_gpu(fin);
    #endif

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
    // cpu bench
    auto start = std::chrono::high_resolution_clock::now();
    unsigned int *liste = find(500000);
    auto stop = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds{stop - start};
    std::cout << "Temps CPU : " << elapsed_seconds.count() << "  secondes" << std::endl;

    // gpu bench
    start = std::chrono::high_resolution_clock::now();
    unsigned int *liste_gpu = find_gpu(500000);
    stop = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds_gpu{stop - start};
    std::cout << "Temps GPU : " << elapsed_seconds_gpu.count() << "  secondes" << std::endl;
    #endif
    
    return EXIT_SUCCESS;
}
