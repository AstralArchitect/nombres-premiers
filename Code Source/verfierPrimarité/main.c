#include <stdio.h>
#include <math.h>

int main(){
    int nombreATester;
    printf("Bonjour,\n");
    printf("Quel nombre voulez-vous tester ?:");
    scanf("%d", &nombreATester);
    int estPremier = 1;
    int why;
    for (int i = 2; i < sqrt(nombreATester); i++)
    {
        if (nombreATester % i == 0)
        {
            why = i;
            estPremier = 0;
            break;
        }
    }
    if (estPremier)
    {
        printf("%i est un nombre premier.", nombreATester);
    }else if (estPremier == 0)
    {
        printf("%i n'est pas un nombre premier, car il est divisible par %d.", nombreATester, why);
    }
}