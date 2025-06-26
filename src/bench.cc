#include <iostream>
#include <chrono>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <string.h>

// functions that search primes
unsigned int *find_n_primes(unsigned int const& fin);
unsigned int *find_to_n(unsigned int const& fin, unsigned int &numPrimesFound);

int main(int argc, char *argv[]) {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    // check arguments
    if (argc != 3) {
        std::cerr << "Usage: ./main [mode] [count]" << std::endl;
        return EXIT_FAILURE;
    }

    // get mode and count
    unsigned int mod   = atoi(argv[1]);
    unsigned int count = atoi(argv[2]);

    unsigned int *primes;

    if (mod == 1) {
        // find primes
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        primes = find_n_primes(count);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - begin;
        std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
        if (primes == NULL) {
            std::cout << "Erreur lors de la recherche des nombres premiers" << std::endl;
            return EXIT_FAILURE;
        }
    } else if (mod == 2) {
        unsigned int numPrimesFound = 0;
        // find primes
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        primes = find_to_n(count, numPrimesFound);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - begin;
        std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
        if (primes == NULL) {
            std::cout << "Erreur lors de la recherche des nombres premiers" << std::endl;
            return EXIT_FAILURE;
        }
    }
    free(primes);
    
    return EXIT_SUCCESS;
}