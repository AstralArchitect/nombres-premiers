#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_SIZE 90000000

int nombresPremiersTrouves = 1;
int fin = 1000;
int liste[MAX_SIZE] = {2};
int *NombresPremiers = &nombresPremiersTrouves;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool estPremier(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0){
            return false;
        }
        else if(n % 2 == 0){
            return false;
        }
    }
    return true;
}

// Thread function to find prime numbers
void *TrouverNombresPremiers(void *arg) {
    int start = *((int *)arg);
    while (true) {
        pthread_mutex_lock(&mutex);
        if (*NombresPremiers >= fin - 3) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        int num = start;
        start += 4;
        pthread_mutex_unlock(&mutex);
        
        if (estPremier(num)) {
            pthread_mutex_lock(&mutex);
            if (*NombresPremiers < fin) {
                liste[*NombresPremiers] = num;
                (*NombresPremiers)++;
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

int main() {
    printf("Combien de nombres premiers voulez-vous chercher ? (max: 49 999 999)");
    scanf("%d", &fin);
    if (fin < MAX_SIZE) {
        pthread_t tid1, tid2, tid3, tid4;
        int start1 = 3, start2 = 7, start3 = 11, start4 = 13; // Starting points for each thread

        // Create threads
        pthread_create(&tid1, NULL, TrouverNombresPremiers, &start1);
        pthread_create(&tid2, NULL, TrouverNombresPremiers, &start2);
        pthread_create(&tid3, NULL, TrouverNombresPremiers, &start3);
        pthread_create(&tid4, NULL, TrouverNombresPremiers, &start4);

        // Wait for threads to finish
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        pthread_join(tid3, NULL);
        pthread_join(tid4, NULL);

        // Print prime numbers
        int ancien = MAX_SIZE;
        for (int j = sizeof(liste)/sizeof(int) - 1; j > 0; j--)
        {
            if(liste[j] < ancien && liste[j] > 0 && liste[j] != MAX_SIZE){
                ancien = liste[j];
                printf("%d\n", liste[j]);
                liste[j] = MAX_SIZE;
            }else{
                ancien = MAX_SIZE;
                continue;
            }
        }
    } else {
        printf("Veuillez entrer un nombre plus petit.");
        exit(EXIT_FAILURE);
    }

    return 0;
}

