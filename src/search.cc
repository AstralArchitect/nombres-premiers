#include <stdlib.h>
#include <stdbool.h>

#include <math.h>

inline bool estPremier(unsigned int const& n, unsigned int *primes) {
    unsigned int root = sqrt(n);
    for (unsigned int i = 2; primes[i] <= root; i++)
        if (n % primes[i] == 0)
            return false;
    
    return true;
}

unsigned int *find(unsigned int const& fin) {
    // variables
    unsigned int *primes;
    unsigned int numPrimesFound = 3;

    // allocation dynamique de mémoire
    if ((primes = (unsigned int*)malloc(fin * sizeof(unsigned int))) == NULL)
        return NULL;
    primes[0] = 2;
    primes[1] = 3;
    primes[2] = 5;
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