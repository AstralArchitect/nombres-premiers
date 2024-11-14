#include <stdlib.h>

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
    unsigned int numPrimesFound = 2;

    // allocation dynamique de mémoire
    if ((primes = (unsigned int*)malloc(fin * sizeof(unsigned int))) == NULL)
        return NULL;
    primes[0] = 2;
    primes[1] = 3;
    unsigned int nTest = 5;
    do
    {
        if (estPremier(nTest, primes))
        {
            primes[numPrimesFound] = nTest;
            numPrimesFound++;
        }
        if (estPremier(nTest + 2, primes))
        {
            primes[numPrimesFound] = nTest + 2;
            numPrimesFound++;
        }
        
        nTest += 6;
    } while (numPrimesFound < fin);
    
    return primes;
}