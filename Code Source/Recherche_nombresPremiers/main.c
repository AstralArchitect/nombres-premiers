#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int nombreATester = 3;
    int nombrePremiersTrouves = 1;
    int fin;
    printf("Bienvenue sur le chercheur de nombres premiers de Astral Architect\n");
    printf("Combien de nombres voulez-vous chercher ?: ");
    scanf("%d", &fin);
    int* liste = (int*)malloc(fin * sizeof(int)); // Allocation dynamique de mémoire
    
    if (liste == NULL) {
        printf("Erreur lors de l'allocation de mémoire.\n");
        return 1;
    }

    while (nombrePremiersTrouves < fin)
    {
        bool estPremier = true;
        for (int j = 0; j < nombrePremiersTrouves; j++)
        {
            if (nombreATester % liste[j] == 0)
            {
                estPremier = false;
                break;
            }
        }
        if (estPremier)
        {
            printf("%d\n", nombreATester);
            liste[nombrePremiersTrouves] = nombreATester;
            nombrePremiersTrouves++;
        }
        nombreATester++;
    }
    free(liste);

    return 0;
}
