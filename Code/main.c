#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_SIZE 90000000
#define NB_THREADS 4

long nombresPremiers = 1;
long fin;
long *liste;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void *print_progression(void *argv){
    long *pointeur = argv;
    long fin = *pointeur;
    while (nombresPremiers <= fin)
    {
        printf("\e[1;1H\e[2J");
        printf("Progression: %.0f%%", ((float)nombresPremiers / fin) * 100);
        fflush(stdout);
        struct timespec ts;
        ts.tv_sec = 500 / 1000;
        ts.tv_nsec = (500 % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
    printf("\n");
    return EXIT_SUCCESS;
}

bool estPremier(long n) {
    for (long i = 2; i * i <= n; i++) {
        if (n % i == 0){
            return false;
        }
    }
    return true;
}

void *thread(void *argv){
    long *pointeur = argv;
    long num = *pointeur;
    while (nombresPremiers <= fin) {
        //pthread_mutex_lock(&mutex);
        if (estPremier(num)) {
            liste[nombresPremiers] = num;
            nombresPremiers++;
        }
	if(NB_THREADS > 1){
		num += 2;
	}
	else if(NB_THREADS == 1){
		num++;
	}
        //pthread_mutex_unlock(&mutex);
    }
    return EXIT_SUCCESS;
}

int main() {
    printf("Combien de nombres premiers voulez-vous chercher ? (max: 90 000 000)");
    scanf("%d", &fin);
    if (fin <= MAX_SIZE) {
        pthread_t print;
        pthread_create(&print, NULL, print_progression, &fin);
        printf("\e[2J");
        liste = malloc(fin * sizeof(long));
        liste[0] = 2;
        if (liste != NULL)
        {
            pthread_t t[NB_THREADS];
            long start[NB_THREADS];
            long startPoint = 3;
            for (long i = 0; i < NB_THREADS; i++)
            {
                start[i] = startPoint;
                startPoint++;
            }
            time_t startTime, stop;
	    startTime = time(NULL);
            for (long i = 0; i < NB_THREADS; i++)
            {
                pthread_create(&t[i], NULL, thread, &start[i]);
            }
            for (long i = 0; i < NB_THREADS; i++)
            {
                pthread_join(t[i], NULL);
            }
	    stop = time(NULL);
            pthread_join(print, NULL);
            printf("\e[2J\e[1;1H");
            printf("La recherche est terminée. En %d secondes.\n\t 1. Enregistrer dans Nombres-Premiers.txt.\n\t2. Tout afficher\n\t3. Afficher et Enregistrer\n:", stop - startTime);
            int rep;
            scanf("%d", &rep);
            if (rep == 1)
            {
                qsort(liste, fin, sizeof(long), cmpfunc);
                FILE *fichier = fopen("Nombres-Premiers.txt", "w+");
                if (fichier != NULL)
                {
                    for (long i = 0; i < fin; i++)
                    {
                        fprintf(fichier, "%ld\n", liste[i]);
                    }
                }
            }
            else if (rep == 2)
            {
                qsort(liste, fin, sizeof(long), cmpfunc);
                for (long i = 0; i < fin; i++)
                {
                    printf("%ld\n", liste[i]);
                }
            }
            else if (rep == 3)
            {
                qsort(liste, fin, sizeof(long), cmpfunc);
                FILE *fichier = fopen("Nombres-Premiers.txt", "w+");
                if (fichier != NULL)
                {
                    for (long i = 0; i < fin; i++)
                    {
                        fprintf(fichier, "%ld\n", liste[i]);
                    }
                }
                for (long i = 0; i < fin; i++)
                {
                    printf("%ld\n", liste[i]);
                }
            }
            else
            {
                printf("Veuillez entrer une réponse correct(1/2).\n");
                exit(EXIT_FAILURE);
            }
        }
        
    } else {
	    printf("\e[0;31m");
            printf("Erreur! Veuillez entrer un nombre plus petit.\n");
	    printf("\e[0;37m");
        exit(EXIT_FAILURE);
    }

    return 0;
}
