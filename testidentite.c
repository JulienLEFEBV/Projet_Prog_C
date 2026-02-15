#include "identite.h"
#include <stdio.h>

int main(void)
{
    FILE *f;
    tIdentite Personne;
    //On teste les fonctions de lecture, d'affichage et de libération 
    f=fopen("personne.ind","rt");
    while((Personne=IdentiteLiref(f))!=NULL) 
    {
        IdentiteAfficher(Personne);
        IdentiteLiberer(Personne);
    }
    fclose(f);
    return 0;
}