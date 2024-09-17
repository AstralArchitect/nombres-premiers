#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <math.h>

bool estPremier(unsigned long n)
{
    if (n % 2 == 0)
    {
        return false;
    }
    
    for (unsigned long j = 3; j <= sqrt(n); j += 2) {
        if (n % j == 0){
            return false;
        }
    }
    return true;
}

void search(unsigned long fin, unsigned long *nombresPremiersTrouves, unsigned long *liste)
{
    unsigned long tid = 3;
    liste[0] = 2;
    while (true)
    {
        if (estPremier(tid))
        {
            liste[*nombresPremiersTrouves] = tid;
            *nombresPremiersTrouves += 1;
        }

        tid++;
        
        if (*nombresPremiersTrouves >= fin)
        {
            break;
        }
    }
}

unsigned long *find(unsigned long fin) {
    // variables
    unsigned long *liste;
    unsigned long nombresPremiers = 1;

    // allocation dynamique de mémoire sur le host
    liste = (unsigned long*)malloc(fin * sizeof(unsigned long));
    if (liste == NULL)
    {
        return NULL;
    }
    
    // recherche
    search(fin, &nombresPremiers, liste);

    return liste;
}