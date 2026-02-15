#include "genea.h"

int main(int argc, char *argv[])
{
    //On teste s'il y a le bon nombre d'arguments
    if(argc!=3)
    {
        fprintf(stderr,"Nombre de paramètres invalides (nombre de paramètres donnés %d, nombre de paramètres demandés 2)\n",argc-1);
        return 1;
    }
    //On teste la fonction de lecture de l'arbre
    tArbre Arbre;
    Arbre=ArbreLirePersonnesFichier(argv[1]);
    if (Arbre==NULL) return 2;
    //On teste la fonction d'affichage
    ArbreAfficher(Arbre);
    //On teste la fonction d'ajout des liens de parentés
    Arbre=ArbreLireLienParenteFichier(Arbre,argv[2]);
    if (Arbre==NULL) return 3;
    ArbreAfficher(Arbre); 
    //On teste la libération de l'arbre
    ArbreLiberer(Arbre);  
    return 0; 
}