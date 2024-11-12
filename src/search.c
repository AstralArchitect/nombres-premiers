#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>

#ifdef _WIN32
#include <windows.h>
#endif

const unsigned char NB_THREADS = 2;

typedef struct 
{
    unsigned int fin;
    unsigned int *NombresPremiersTrouves;
    unsigned int *liste;
    int stride;
    int tid;
} ThreadInfo;

pthread_mutex_t mutex;

// https://code.letuscrack.com/prime-numbers-ranges/
bool estPremier(int n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }
    int i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
        i += 6;
    }
    return true;
}

void *search(void *argv) {
    ThreadInfo *threadInfo = (ThreadInfo *)argv;

    while (*threadInfo->NombresPremiersTrouves < threadInfo->fin) {
        if (estPremier(threadInfo->tid)) {
            pthread_mutex_lock(&mutex);
            threadInfo->liste[*threadInfo->NombresPremiersTrouves] = threadInfo->tid;
            (*threadInfo->NombresPremiersTrouves)++;
            pthread_mutex_unlock(&mutex);
        }

        threadInfo->tid += threadInfo->stride;
    }

    return NULL;
}

unsigned int *find(unsigned int fin) {
    // variables
    unsigned int *primes;
    unsigned int numPrimesFound = 0;

    // allocation dynamique de mémoire sur le host
    primes = (unsigned int*)malloc(fin * sizeof(unsigned int));
    if (primes == NULL)
    {
        return NULL;
    }

    // parametrage des threads
    pthread_t t[NB_THREADS];
    ThreadInfo threadInfos[NB_THREADS];

    pthread_mutex_init(&mutex, NULL);

    for (unsigned char i = 0; i < NB_THREADS; i++)
    {
        threadInfos[i].fin = fin;
        threadInfos[i].liste = primes;
        threadInfos[i].NombresPremiersTrouves = &numPrimesFound;
        threadInfos[i].stride = NB_THREADS;
        threadInfos[i].tid = i;
        
        pthread_create(&t[i], NULL, search, &threadInfos[i]);
    }
    
    // wait for threads finish
    for (unsigned char i = 0; i < NB_THREADS; i++)
    {
        pthread_join(t[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    
    return primes;
}