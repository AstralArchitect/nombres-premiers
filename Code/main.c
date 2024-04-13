#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
//ce nombre correspond au nombres max de nombres premiers. Vous pouvez le changer a votre guise
#define MAX_SIZE 90000000
//Attention, ne pas changer le nombre de threads au dela de 2 car cela entrenera un comportement imprevu du programme
#define NB_THREADS 2
//nombre de nombres premiers trouves
long nombresPremiers = 1;
//valeur d'arret de la recherche
long fin;
//liste de nombres premiers
long *liste;

//fonction pour comparer 2 nombres. Elle sera utilisee pour trier la liste
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
//fonction du thread qui affichera la progression (en %)
void *print_progression(void *argv){
    //recuperation de la valeur de fin
    long *pointeur = argv;
    long fin = *pointeur;
    //boucle qui s'arrete lorsque la recherche est terminee
    while (nombresPremiers <= fin)
    {
	//effacer le terminal et positionner le pointeur au debu
        printf("\e[1;1H\e[2J");
	//afficher la progression
        printf("Progression: %.0f%%", ((float)nombresPremiers / fin) * 100);
	//nessessaire pour que l'on voit le texte qui s'affiche
	fflush(stdout);
	//attendre une demi-seconde
        struct timespec ts;
        ts.tv_sec = 500 / 1000;
        ts.tv_nsec = (500 % 1000) * 1000000;
        nanosleep(&ts, NULL);
    }
    //afficher un retour a la ligne
    printf("\n");
    return EXIT_SUCCESS;
}
//fonction pour determiner si un nombre est premier
bool estPremier(long n) {
    //répéter tant que i est en dessous de la racine du nombre (i x i est plus petit ou égal au nombre)
    for (long i = 2; i * i <= n; i++) {
        if (n % i == 0){
            return false;
        }
    }
    return true;
}
//threads de calcul
void *thread(void *argv){
    //recuperer la valeur de depart
    long *pointeur = argv;
    long num = *pointeur;
    //boucle
    while (nombresPremiers <= fin) {
	//si le nombre est premier alors...
        if (estPremier(num)) {
	        //l'ajouter à la liste des nombres premiers
            liste[nombresPremiers] = num;
            //puis incrémenter de 1 le nombre de nombres premiers trouvés
            nombresPremiers++;
        }
	    num += NB_THREADS;
    }
    return EXIT_SUCCESS;
}

int main() {
    //demander à l'utilisateur combien de nombres
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
