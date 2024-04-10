#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 90000000

int nombresPremiers = 1;
int fin = 1000;
int liste[MAX_SIZE] = {2};

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

int main() {
    printf("Combien de nombres premiers voulez-vous chercher ? (max: 49 999 999)");
    scanf("%d", &fin);
    if (fin < MAX_SIZE) {
        while (nombresPremiers >= fin) {
        int num = start;
        start++;
        
        if (estPremier(num)) {
            if (*NombresPremiers < fin) {
                liste[*NombresPremiers] = num;
                (*NombresPremiers)++;
            }
        }
				}
    } else {
        printf("Veuillez entrer un nombre plus petit.");
        exit(EXIT_FAILURE);
    }

    return 0;
}

