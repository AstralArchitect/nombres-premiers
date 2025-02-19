#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>

#include <pthread.h>

#define NB_THREADS 4

typedef struct {
    unsigned int id;
    pthread_mutex_t *primeMutex;
    pthread_barrier_t *barrier;
}ThreadInfo;

volatile unsigned int numPrimesFound = 2;
unsigned int *primes;
unsigned int fin;

unsigned int currentCandidate = 3;

pthread_mutex_t resultMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t candidate_mutex = PTHREAD_MUTEX_INITIALIZER;

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
    ThreadInfo *info = (ThreadInfo*)arg;
    
    printf("Thread id (create): %d\n", info->id);
    pthread_barrier_wait(info->barrier);

    while (1)
    {
        // first, check if we have found enough primes number
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
        currentCandidate += 2; // Only odd numbers are considered.
        pthread_mutex_unlock(&candidate_mutex);

        if (estPremier(candidate))
        {
            // critical section
            pthread_mutex_lock(info->primeMutex);
            if (numPrimesFound < fin) { // Check again to be safe.
                primes[numPrimesFound] = candidate;
                numPrimesFound++;
            }
            pthread_mutex_unlock(info->primeMutex);
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

    // créer la liste des info de thread
    ThreadInfo infos[NB_THREADS];
    pthread_t threads[NB_THREADS];

    pthread_mutex_t primeMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, NULL, NB_THREADS);
    
    for (unsigned int i = 0; i < NB_THREADS; i++)
    {
        // first, initialize infos
        infos[i].id = i;
        infos[i].primeMutex = &primeMutex;
        infos[i].barrier = &barrier;

        // then launch
        pthread_create(&threads[i], NULL, thread, &infos[i]);
    }

    // wait for threads to finish
    for (unsigned int i = 0; i < NB_THREADS; i++)
        pthread_join(threads[i], NULL);

    // destroy pthread ressources
    pthread_mutex_destroy(&primeMutex);
    pthread_barrier_destroy(&barrier);

    return primes;
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

// Global variables
unsigned long *primes; // Array to store found primes
int num_primes; // Total number of primes to find (x)
int found = 0; // Number of primes found so far

// Starting candidate (only odd numbers are checked)
unsigned long currentCandidate = 3;

// Mutexes for synchronizing candidate and result updates
pthread_mutex_t candidate_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to check if a number is prime.
// Assumes num is odd (except when num is 2).
bool isPrime(unsigned long num) {
    if (num < 2) return false;
    if (num % 2 == 0) return (num == 2);
    unsigned long limit = (unsigned long) sqrt(num);
    for (unsigned long i = 3; i <= limit; i += 2) {
        if (num % i == 0)
            return false;
    }
    return true;
}

// Worker thread function: repeatedly pick a candidate and check for primality.
void* search_primes(void *arg) {
    while (1) {
        // First, check if we've already found enough primes.
        pthread_mutex_lock(&result_mutex);
        if (found >= num_primes) {
            pthread_mutex_unlock(&result_mutex);
            break;
        }
        pthread_mutex_unlock(&result_mutex);

        // Get the next candidate number (skip even numbers).
        pthread_mutex_lock(&candidate_mutex);
        unsigned long candidate = currentCandidate;
        currentCandidate += 2; // Only odd numbers are considered.
        pthread_mutex_unlock(&candidate_mutex);

        // Check if the candidate is prime.
        if (isPrime(candidate)) {
            pthread_mutex_lock(&result_mutex);
            if (found < num_primes) { // Check again to be safe.
                primes[found] = candidate;
                found++;
            }
            pthread_mutex_unlock(&result_mutex);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_primes> <number_of_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    num_primes = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    if (num_primes <= 0 || num_threads <= 0) {
        fprintf(stderr, "Both number of primes and threads must be positive integers.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the primes array.
    primes = malloc(num_primes * sizeof(unsigned long));
    if (primes == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Add 2 as the first prime (if needed).
    if (num_primes > 0) {
        primes[0] = 2;
        found = 1;
    }

    // Create threads.
    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&threads[i], NULL, search_primes, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish.
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the found primes.
    printf("Found %d primes:\n", num_primes);
    for (int i = 0; i < num_primes; i++) {
        printf("%lu ", primes[i]);
    }
    printf("\n");

    // Clean up.
    free(primes);
    free(threads);
    pthread_mutex_destroy(&candidate_mutex);
    pthread_mutex_destroy(&result_mutex);

    return 0;
}
*/