#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

inline bool estPremier(unsigned int n, unsigned int *primes) {
    static bool first = true;
    if(first){
        if (n <= 3) {
            return true;
        }
        first = false;
    }
    int i = 0;
    while (primes[i] * primes[i] <= n) {
        if (n % primes[i] == 0) {
            return false;
        }
        i++;
    }
    return true;
}

extern "C" unsigned int *find(unsigned int fin) {
    // variables
    unsigned int *primes;
    unsigned int numPrimesFound = 0;

    // allocation dynamique de mémoire
    if ((primes = (unsigned int*)malloc(fin * sizeof(unsigned int))) == NULL)
        return NULL;
    
    unsigned int nTest = 2;
    do
    {
        if (estPremier(nTest, primes))
        {
            primes[numPrimesFound] = nTest;
            numPrimesFound++;
        }
        nTest++;
    } while (numPrimesFound < fin);
    
    return primes;
}