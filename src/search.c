#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <mutexManager.h>

// modifier selon le nombre de coeur de votre processeur
const unsigned char nb_threads = 2;

typedef struct 
{
    unsigned long fin;
    MutexedVar *NombresPremiersTrouves;
    unsigned long *liste;
    int stride;
    int tid;
} ThreadInfo;

bool estPremier(unsigned long n)
{
    if (n % 2 == 0)
    {
        return false;
    }
    
    for (unsigned long j = 3; j * j <= n; j += 2) {
        if (n % j == 0){
            return false;
        }
    }
    return true;
}

void *search(void *argv)
{
    ThreadInfo *pointer = argv;
    ThreadInfo threadInfo = *pointer;
    while (true)
    {
        if (estPremier(threadInfo.tid))
        {
            threadInfo.liste[threadInfo.NombresPremiersTrouves->var] = threadInfo.tid;
            increase(threadInfo.NombresPremiersTrouves, 1);
        }

        threadInfo.tid += threadInfo.stride;
        
        if(threadInfo.NombresPremiersTrouves->var >= threadInfo.fin)
        {
            break;
        }
    }
    return EXIT_SUCCESS;
}

unsigned long *find(unsigned long fin) {
    // variables
    unsigned long *liste;
    MutexedVar nombresPremiers;
    initVar(&nombresPremiers, 1);

    // allocation dynamique de mémoire sur le host
    liste = (unsigned long*)malloc(fin * sizeof(unsigned long));
    if (liste == NULL)
    {
        return NULL;
    }
    liste[0] = 2;

    // parametrage des threads
    ThreadInfo threads[nb_threads];
    pthread_t t[nb_threads];
    int actualTid = 3;
    for (unsigned char i = 0; i < nb_threads; i++)
    {
        threads[i].fin = fin;
        threads[i].liste = liste;
        threads[i].NombresPremiersTrouves = &nombresPremiers;
        threads[i].stride = nb_threads;
        threads[i].tid = actualTid;
        actualTid++;
    }

    // recherche
    for (unsigned char i = 0; i < nb_threads; i++)
    {
        pthread_create(&t[i], NULL, search, (void*)&threads[i]);
    }
    
    // wait for threads finish
    for (unsigned char i = 0; i < nb_threads; i++)
    {
        pthread_join(t[i], NULL);
    }
    
    return liste;
}