#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_SIZE 50000000

int nombresPremiersTrouves = 1;
int fin = 1000;
int liste[MAX_SIZE] = {2};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool estPremier(int n) {
    if (n <= 1)
        return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Thread function to find prime numbers
void *TrouverNombresPremiers(void *arg) {
    int start = *((int *)arg);
    int *NombresPremiers = &nombresPremiersTrouves;
    for (int num = start; *NombresPremiers < fin; num += 4) {
        if (estPremier(num)) {
            pthread_mutex_lock(&mutex);
            liste[(*NombresPremiers)++] = num;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

int main() {
    printf("Combien de nombres premiers voulez-vous chercher ? (max: 499 999 999)");
    scanf("%d", &fin);
    char pass = '0';
    if(fin < MAX_SIZE){
        pthread_t tid1, tid2, tid3, tid4;
        int start1 = 3, start2 = 5, start3 = 7, start4 = 11; // Starting points for each thread

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
        for(int i = 0; i < fin; i++) {
        printf("%d\n", liste[i]);
        }
    }else{
        printf("Veuillez entrer un nombre plus petit.");
        exit( EXIT_SUCCESS );
    }

    return 0;
}
