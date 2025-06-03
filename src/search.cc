#include <stdlib.h>
#include <string.h>
#include <thread>
#include <atomic>

inline bool estPremier(unsigned int const& n, unsigned int *primes) {
    for (unsigned int i = 2; primes[i] * primes[i] <= n; i++)
        if (n % primes[i] == 0)
            return false;
    
    return true;
}

inline bool estPremier(unsigned int const& n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;

    if (n % 2 == 0) return false;
    if (n % 3 == 0) return false;

    for (unsigned int i = 5; i * i < n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

unsigned int *find_n_primes(unsigned int const& fin) {
    // Handle cases where 0 primes are requested.
    if (fin == 0) return NULL;

    // variables
    unsigned int *primes;
    unsigned int numPrimesFound = 2;

    // allocation dynamique de mémoire
    if ((primes = (unsigned int*)malloc((fin + 1) * sizeof(unsigned int))) == NULL)
        return NULL;
    primes[0] = 2;
    if (fin == 1) return primes;

    primes[1] = 3;
    if (fin == 2) return primes;

    primes[2] = 5;
    unsigned int nTest = 5;
    while (numPrimesFound < fin){
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
    }
    
    return primes;
}

std::atomic<int> nbPrimesFound(1);
const unsigned char THREADS = 20;

void find_to_n_thread(unsigned int const& fin, unsigned short const& id, bool *isPrimes) {
    unsigned int nbPrimesFound_local = 0;
    for (unsigned int i = id; i < fin; i += THREADS){
        if (estPremier(i * 2 + 3))
        {
            isPrimes[i] = true;
            nbPrimesFound_local++;
        }
    }
    nbPrimesFound += nbPrimesFound_local;
}

unsigned int *find_to_n(unsigned int const& fin, unsigned int &numPrimesFound) {
    // Handle cases where 0 primes are requested.
    if (fin == 0) return NULL;

    // initialisation du tableau
    bool *isPrimes = new bool[fin / 2];

    memset(isPrimes, 0, (fin / 2) * sizeof(bool)); 

    // primes test
    std::thread threads[THREADS];
    for (unsigned short id = 0; id < THREADS; ++id) {
        threads[id] = std::thread(find_to_n_thread, fin / 2, id, isPrimes);
    }
    for (unsigned short id = 0; id < THREADS; ++id) {
        threads[id].join();
    }

    // transform to int array
    unsigned int *primes = new unsigned int[fin];
    primes[0] = 2;
    numPrimesFound = 1;
    for (int i = 0; i < fin / 2; i++) {
        if (!isPrimes[i]) continue;
        primes[numPrimesFound] = i * 2 + 3;
        numPrimesFound++;
    }

    delete [] isPrimes;
    return primes;
}