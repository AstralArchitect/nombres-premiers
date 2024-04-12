#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 90000000
#define NB_THREADS 2

int nombresPremiers = 1;
int fin;
unsigned long *liste;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void *print_progression(void *argv){
    int *pointeur = argv;
    int fin = *pointeur;
    while (nombresPremiers <= fin)
    {
        printf("\e[1;1H");
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

bool estPremier(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0){
            return false;
        }
    }
    return true;
}

void *thread(void *argv){
    unsigned long *pointeur = argv;
    unsigned long num = *pointeur;
    while (nombresPremiers <= fin) {
        pthread_mutex_lock(&mutex);
        if (estPremier(num)) {
            liste[nombresPremiers] = num;
            nombresPremiers++;
        }
        num += NB_THREADS;
        pthread_mutex_unlock(&mutex);
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
        liste = malloc(fin * sizeof(unsigned long));
        liste[0] = 2;
        if (liste != NULL)
        {
            pthread_t t[NB_THREADS];
            int start[NB_THREADS];
            int startPoint = 3;
            for (int i = 0; i < NB_THREADS; i++)
            {
                start[i] = startPoint;
                startPoint++;
            }
            
            for (int i = 0; i < NB_THREADS; i++)
            {
                pthread_create(&t[i], NULL, thread, &start[i]);
            }
            for (int i = 0; i < NB_THREADS; i++)
            {
                pthread_join(t[i], NULL);
            }
            pthread_join(print, NULL);
            printf("\e[2J\e[1;1H");
            printf("La recherche est terminée.\n\t1. Enregistrer dans Nombres-Premiers.txt.\n\t2. Tout afficher\n\t3. Afficher et Enregistrer\n:");
            int rep;
            scanf("%d", &rep);
            if (rep == 1)
            {
                qsort(liste, fin, sizeof(int), cmpfunc);
                FILE *fichier = fopen("Nombres-Premiers.txt", "w+");
                if (fichier != NULL)
                {
                    for (int i = 0; i < fin; i++)
                    {
                        fprintf(fichier, "%d, ", liste[i]);
                    }
                }
            }
            else if (rep == 2)
            {
                qsort(liste, fin, sizeof(int), cmpfunc);
                for (int i = 0; i < fin; i++)
                {
                    printf("%d\n", liste[i]);
                }
            }
            else if (rep == 3)
            {
                qsort(liste, fin, sizeof(int), cmpfunc);
                FILE *fichier = fopen("Nombres-Premiers.txt", "w+");
                if (fichier != NULL)
                {
                    for (int i = 0; i < fin; i++)
                    {
                        fprintf(fichier, "%d, ", liste[i]);
                    }
                }
                for (int i = 0; i < fin; i++)
                {
                    printf("%d\n", liste[i]);
                }
            }
            else
            {
                printf("Veuillez entrer une réponse correct(1/2).");
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
