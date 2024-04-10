#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_SIZE 90000000

int nombresPremiers = 1;
int fin;

bool estPremier(int n) {
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0){
            return false;
        }
    }
    return true;
}

int main() {
    printf("Combien de nombres premiers voulez-vous chercher ? (max: 49 999 999)");
    scanf("%d", &fin);
    if (fin < MAX_SIZE) {
	int num = 3;
        while (nombresPremiers <= fin) {
            num++;
        
            if (estPremier(num)) {
                liste[nombresPremiers] = num;
                nombresPremiers++;
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
