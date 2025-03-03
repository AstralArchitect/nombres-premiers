#include <stdlib.h>
#include <stdbool.h>

#include <pthread.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

volatile unsigned int numPrimesFound = 2;
unsigned int *primes;
unsigned int fin;

unsigned int currentCandidate = 5;

pthread_mutex_t resultMutex;
pthread_mutex_t candidate_mutex;
pthread_mutex_t primes_mutex;

inline bool estPremier(unsigned int const& n) {
    if (n % 2 == 0 || n % 3 == 0)
        return false;

    for (unsigned int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0)
            return false;
        if (n % (i + 2) == 0)
            return false;
    }
    
    return true;
}

void *thread(void *arg)
{
    while (1)
    {
        // first, check if we found enough primes number
        pthread_mutex_lock(&resultMutex);
        if (numPrimesFound >= fin)
        {
            pthread_mutex_unlock(&resultMutex);
            break;
        }
        pthread_mutex_unlock(&resultMutex);

        // Get the next candidate number (skip even numbers).
        pthread_mutex_lock(&candidate_mutex);
        unsigned int candidate = currentCandidate;
        currentCandidate += 6; // Only odd numbers are considered.
        pthread_mutex_unlock(&candidate_mutex);

        if (estPremier(candidate))
        {
            // critical section
            pthread_mutex_lock(&primes_mutex);
            if (numPrimesFound < fin) { // Check again to be safe.
                primes[numPrimesFound] = candidate;
                numPrimesFound++;
            }
            pthread_mutex_unlock(&primes_mutex);
        }

        if (estPremier(candidate + 2))
        {
            // critical section
            pthread_mutex_lock(&primes_mutex);
            if (numPrimesFound < fin) { // Check again to be safe.
                primes[numPrimesFound] = candidate + 2;
                numPrimesFound++;
            }
            pthread_mutex_unlock(&primes_mutex);
        }
    }
    
    return EXIT_SUCCESS;
}

unsigned int *find(unsigned int const& fin_loc) {
    fin = fin_loc;
    // allocation dynamique de mémoire
    if ((primes = (unsigned int*)malloc((fin) * sizeof(unsigned int))) == NULL)
        return NULL;
    primes[0] = 2;
    primes[1] = 3;

    // get the processor's num of threads
    int num_threads;

#ifdef _WIN32 // Get number of hardware threads on Windows
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    num_threads = sysinfo.dwNumberOfProcessors;
#else // Get number of hardware threads on POSIX systems
    num_threads = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_threads == -1) // Handle error if necessary
        num_threads = 4; // Fallback value
#endif

    // créer la liste des info de thread
    pthread_t threads[num_threads];

    pthread_mutex_init(&primes_mutex, NULL);
    pthread_mutex_init(&candidate_mutex, NULL);
    pthread_mutex_init(&primes_mutex, NULL);
    
    // launch threads
    for (int i = 0; i < num_threads; i++)
        pthread_create(&threads[i], NULL, thread, NULL);

    // wait for threads to finish
    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    // destroy pthread ressources
    pthread_mutex_destroy(&primes_mutex);
    pthread_mutex_destroy(&candidate_mutex);
    pthread_mutex_destroy(&resultMutex);

    return primes;
}