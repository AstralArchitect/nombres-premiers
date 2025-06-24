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
    if (n % 3 == 0) return false;

    for (unsigned int i = 5; i * i < n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
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

unsigned int THREADS;

void find_to_n_thread(unsigned int const& array_size, unsigned short const& id, bool *isPrimes) {
    for (unsigned int i = id; i < array_size; i += THREADS){
        if (estPremier(i * 2 + 5)) // the candidate is : i * 2 + 5
        {
            isPrimes[i] = true;
        }
    }
}

unsigned int *find_to_n(unsigned int const& fin, unsigned int &numPrimesFound) {
    // Handle cases where 0 primes are requested.
    if (fin == 0) return NULL;

    // initialize an array of bool which represent numbers from 5 (included) to fin jumping pair numbers
    unsigned int array_lenth = (fin - 5) / 2;
    bool *isPrimes = new bool[array_lenth];

    memset(isPrimes, 0, array_lenth * sizeof(bool)); 

    // get the number of threads of the processor
    const unsigned int processor_count = std::thread::hardware_concurrency();
    THREADS = processor_count > 0 ? processor_count : 1;

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
    // set the firsts primes
    primes[0] = 2;
    primes[1] = 3;
    numPrimesFound = 2; // found 2 & 3
    for (int i = 0; i < array_lenth; i++) { // i is the index of isPrimes
        if (!isPrimes[i]) continue;
        primes[numPrimesFound] = i * 2 + 5;
        numPrimesFound++;
    }

    delete [] isPrimes;
    return primes;
}