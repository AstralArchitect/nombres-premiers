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

// functions that print arrays
void printCArrayFwrite(const unsigned int* arr, size_t size);

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
        printCArrayFwrite(primes, fin);
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
        printCArrayFwrite(primes, numPrimesFound);
    }
    free(primes);
    
    return EXIT_SUCCESS;
}


// A fast integer-to-string conversion function (itoa)
inline char* fast_itoa(unsigned int n, char* s) {
    if (n == 0) {
        *s++ = '0';
        return s;
    }
    char* start = s;
    while (n > 0) {
        *s++ = (n % 10) + '0';
        n /= 10;
    }
    std::reverse(start, s);
    return s;
}

void printCArrayFwrite(const unsigned int* arr, size_t size) {
    // Estimate buffer size. Max 11 chars for a 32-bit int (-2147483648)
    // plus a newline character.
    char *buffer = new char[size * 12];
    char* ptr = buffer;

    for (size_t i = 0; i < size; ++i) {
        ptr = fast_itoa(arr[i], ptr);
        *ptr++ = '\n';
    }

    // Write the entire buffer to stdout in one go
    fwrite(buffer, 1, ptr - buffer, stdout);
    delete [] buffer;
}