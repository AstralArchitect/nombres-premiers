#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 90000000

int nombresPremiers = 1;
int fin;

void sleep_ms(unsigned long milliseconds) {
    struct timespec ts;

    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    nanosleep(&ts, NULL);
}

void *print_progression(void *argv){
    int *pointeur = argv;
    int fin = *pointeur;
    while (nombresPremiers <= fin)
    {
        printf("\e[1;1H");
        printf("Progression: %.0f%%", ((float)nombresPremiers / fin) * 100);
        fflush(stdout);
        sleep_ms(500);
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

int main() {
    printf("Combien de nombres premiers voulez-vous chercher ? (max: 90 000 000)");
    scanf("%d", &fin);
    if (fin <= MAX_SIZE) {
        pthread_t t1;
        pthread_create(&t1, NULL, print_progression, &fin);
        printf("\e[2J");
        int *liste = malloc(fin * sizeof(int));
        if (liste != NULL)
        {
            int num = 3;
            while (nombresPremiers <= fin) {
                if (estPremier(num)) {
                    liste[nombresPremiers] = num;
                    nombresPremiers++;
	            }
                num++;
	        }
            pthread_join(t1, NULL);
            printf("\e[2J\e[1;1H");
            printf("La recherche est terminée, voulez-vous l'enregistrer dans Nombres-Premiers.txt. Si le fichier est déja éxistant, les donnés seront écrasés ?([O]ui/[n]on):");
            fgetc(stdin);
            char rep = fgetc(stdin);
            if (rep == 'O' || rep == 'o')
            {
                FILE *fichier = fopen("Nombres-Premiers.txt", "w+");
                if (fichier != NULL)
                {
                    for (int i = 0; i < fin; i++)
                    {
                        fprintf(fichier, "%d, ", liste[i]);
                    }
                }
            
            }
            else if (rep == 'n' || rep == 'N')
            {
                printf("Annumation...\n");
                exit(EXIT_SUCCESS);
            }
            
            else
            {
                printf("Veuillez entrer une réponse correct(O/n).");
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
