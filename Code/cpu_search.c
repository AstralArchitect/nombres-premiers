#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define NB_THREAD 2

typedef struct {
    int startPoint;
    unsigned long *liste;
} ThreadInfo;

unsigned long fin_global;
unsigned long nombresPremiersTrouves = 0L;

bool cpu_estPremier(unsigned long n)
{
    for (unsigned long j = 2; j * j <= n; j++) {
        if (n % j == 0){
            return false;
            break;
        }
    }
    return true;
}

void *thread(void *pointer)
{
    ThreadInfo *thisThreadInfo = (ThreadInfo*)pointer;
    for (int i = thisThreadInfo->startPoint; nombresPremiersTrouves < fin_global; i += NB_THREAD) {
        if (cpu_estPremier(i)) {
            nombresPremiersTrouves++;
            thisThreadInfo->liste[nombresPremiersTrouves] = i;
        }
    }

    return EXIT_SUCCESS;
}

unsigned long *find_cpu(unsigned long fin)
{
    unsigned long *liste = (unsigned long*)malloc(fin * sizeof(unsigned long));
    if (liste == NULL)
    {
        return NULL;
    }

    liste[0] = 2;

    fin_global = fin;

    ThreadInfo threadsInfos[NB_THREAD];
    int firstStartPoint = 3;
    
    for (int i = 0; i < NB_THREAD; i++)
    {
        threadsInfos[i].liste = liste;
        threadsInfos[i].startPoint = firstStartPoint;

        firstStartPoint++;
    }

    pthread_t t[NB_THREAD];
    for (int i = 0; i < NB_THREAD; i++)
    {
        pthread_create(&t[i], NULL, thread, (void*)&threadsInfos[i]);
    }

    for (int i = 0; i < NB_THREAD; i++)
    {
        pthread_join(t[i], NULL);
    }

    return liste;
}