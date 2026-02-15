#include "genea.h"

int main(int argc, char *argv[])
{
    //On vérifie le nombre de paramètres
    if(argc!=4)
    {
        fprintf(stderr,"Nombre de paramètres invalides (nombre de paramètres donnés %d, nombre de paramètres demandés 3)\n",argc-1);
        return 1;
    }
    tArbre Arbre,Arbre2;
    Arbre=ArbreLirePersonnesFichier(argv[1]);
    if (Arbre==NULL) return 2; //On retourne un code d'erreur s'il y a un problème de lecture 
    Arbre2=ArbreLireLienParenteFichier(Arbre,argv[2]);
    if (Arbre2==NULL) //On retourne un code d'erreur s'il y a un problème de lecture 
    {
        ArbreLiberer(Arbre);
        return 3;
    }
    Arbre=Arbre2;
    ArbreEcrireGV(Arbre, argv[3]);
    ArbreLiberer(Arbre);  
    return 0; 
}