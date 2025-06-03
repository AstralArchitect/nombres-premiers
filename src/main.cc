#include <iostream>
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

// functions that search primes
unsigned int *find_n_primes(unsigned int const& fin);
unsigned int *find_to_n(unsigned int const& fin, unsigned int &numPrimesFound);

int main(int argc, char *argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    // first, ask for the mod the user want to enter
    std::cout << "Quel mode voulez-vous utiliser ?" << std::endl;
    std::cout << "1. n premiers nombres premiers" << std::endl;
    std::cout << "2. nombres premier de 0 à n" << std::endl;

    int mod = 0;
    std::cin >> mod;

    unsigned int *primes;

    if (mod == 1) {
        // ask for the number
        unsigned int fin;
        std::cout << "Entrez le nombre de nombres premiers que vous voulez trouver" << std::endl;
        std::cin >> fin;
        // find primes
        primes = find_n_primes(fin);
        if (primes == NULL) {
            std::cout << "Erreur lors de la recherche des nombres premiers" << std::endl;
            return EXIT_FAILURE;
        }
        // print primes
        for (unsigned int i = 0; i < fin; i++) {
            std::cout << primes[i] << std::endl;
        }
    } else if (mod == 2) {
        // ask for the number
        unsigned int fin;
        unsigned int numPrimesFound;
        std::cout << "Entrez la limite des nombres premiers a trouver" << std::endl;
        std::cin >> fin;
        // find primes
        primes = find_to_n(fin, numPrimesFound);
        if (primes == NULL) {
            std::cout << "Erreur lors de la recherche des nombres premiers" << std::endl;
            return EXIT_FAILURE;
        }
        // print primes
        for (unsigned int i = 0; i < numPrimesFound; i++) {
            std::cout << primes[i] << std::endl;
        }
    }
    free(primes);
    
    return EXIT_SUCCESS;
}
