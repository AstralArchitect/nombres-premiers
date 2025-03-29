#include <iostream>
#include <cstdlib>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <string>
#ifdef BENCH
#include <chrono>
#endif

unsigned int* find(unsigned int const& fin);
unsigned int* find_gpu(unsigned int const& fin);

int main(int argc, char *argv[]) {
    #ifndef BENCH
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    unsigned int fin;
    bool preDefinedEnd = argc > 1 ? true : false;
    if (preDefinedEnd)
    {
        std::sscanf(argv[1], "%d", &fin);
    }
    else
    {
        std::cout << "Combien de nombres premiers voulez-vous chercher ?: ";
        std::cin >> fin;
    }
    std::cout << "recherche...";
    #ifndef CUDA
    unsigned int *liste = find(fin);
    #else
    unsigned int *liste;
    if (fin < 500000)
        liste = find(fin);
    else
        liste = find_gpu(fin);
    #endif

    if (!liste)
    {
        std::cout << "\033[0;31m";
        std::cout << "Une erreur s'est produite\n";
        std::cout << "\033[0;37m";
        return EXIT_FAILURE;
    }

    std::cout << "\033[32m";
    std::cout << "recherche terminée!" << std::endl;
    std::cout << "\033[37m";

    for (unsigned int i = 0; i < fin; i++)
    {
        std::cout << liste[i] << std::endl;
    }

    free(liste);
    #else

    #ifdef CUDA
    // gpu bench
    unsigned int *liste_gpu = find_gpu(1000000);
    #else
    // cpu bench
    unsigned int *liste = find(1000000);
    #endif

    #endif
    
    return EXIT_SUCCESS;
}