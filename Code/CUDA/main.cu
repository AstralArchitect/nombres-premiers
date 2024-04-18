#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <cuda.h>
#include <cuda_runtime.h>

#define MAX_SIZE 2000000000

long *liste, *liste_d;
int nombresPremiers, *nombresPremiers_d;

__global__ void thread(long fin, int *nombresPremiersTrouves, long *liste){
    int tid = blockIdx.x * blockDim.x + threadIdx.x + 2;
    int estPremier;

    while (true) {
        estPremier = 1;
        for (long j = 2; j * j <= tid; j++) {
            if (tid % j == 0){
                estPremier = 0;
                break;
            }
        }
        if (estPremier) {
            int idx = atomicAdd(nombresPremiersTrouves, 1);
            if (idx < fin) {
                liste[idx] = tid;
            }
        }
        tid += gridDim.x * blockDim.x;

        if (*nombresPremiersTrouves >= fin) {
            break;
        }
    }
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
    //demander à l'utilisateur combien de nombres veut-il chercher et enregistrer la réponse dans fin
    printf("Combien de nombres premiers voulez-vous chercher ? (max: 2 000 000 000)");
    long fin, *fin_d;
    scanf("%ld", &fin);
    if (fin <= MAX_SIZE) {
        printf("\033[2J\033[H");
        cudaMalloc(&liste_d, fin * sizeof(long));
        cudaMalloc(&fin_d, sizeof(long));
        cudaMalloc(&nombresPremiers_d, sizeof(long));
        if (liste_d != NULL)
        {
            time_t startTime, stop;
	        startTime = time(NULL);

            int *nombresPremiers_d;
            cudaMalloc(&nombresPremiers_d, sizeof(int));

            int init = 0;
            cudaMemcpy(nombresPremiers_d, &init, sizeof(int), cudaMemcpyHostToDevice);

            int block_size = 256;
            int grid_size = ((fin + block_size) / block_size);
            thread<<<grid_size,block_size>>>(fin, nombresPremiers_d, liste_d);

            liste = (long*)malloc(fin * sizeof(long));

            if (liste == NULL)
            {
                printf("\033[0;31m");
                printf("Impossible d'effectuer l'allocation dynamique de mémoire sur le CPU\n");
                printf("\033[0;37m");
                exit(EXIT_FAILURE);
            }

	        cudaMemcpy(liste, liste_d, fin * sizeof(long), cudaMemcpyDeviceToHost);

            cudaFree(nombresPremiers_d);
            cudaFree(liste_d);

            stop = time(NULL);
            printf("\033[2J\033[1;1H");
            printf("La recherche est terminée. En %lld secondes.\n\t1. Enregistrer dans Nombres-Premiers.txt.\n\t2. Tout afficher\n\t3. Afficher et Enregistrer\n:", stop - startTime);
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
                printf("Veuillez entrer une réponse correct(1/2/3).\n");
                exit(EXIT_FAILURE);
            }
            free(liste);
        }
        else{
            printf("\033[0;31m");
            printf("Erreur! Impossible d'effectuer l'allocation dynamique de mémoire sur le GPU\n");
	        printf("\033[0;37m");
            exit(EXIT_FAILURE);
        }
    } else {
	    printf("\033[0;31m");
        printf("Erreur! Votre nombre est incorrect, veuillez entrer un nombre plus petit que %d\n", MAX_SIZE);
	    printf("\033[0;37m");
        exit(EXIT_FAILURE);
    }

    return 0;
}