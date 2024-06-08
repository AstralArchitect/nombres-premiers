#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

unsigned long *find_cpu(unsigned long fin)
{
    unsigned long *liste = (unsigned long*)malloc(fin * sizeof(unsigned long));
    if (liste == NULL)
    {
        return NULL;
    }

    liste[0] = 2;

    unsigned long nombresPremiersTrouves = 0;
    for (int i = 3; nombresPremiersTrouves <= fin; i++) {
        if (cpu_estPremier(i)) {
            nombresPremiersTrouves++;
            liste[nombresPremiersTrouves] = i;
        }
    }

    return liste;
}