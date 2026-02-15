#include <stdio.h>
#include <stdlib.h>
#include "identite.h"

#define LG_DATE 10 // Nombre de caractères d ’une date sous la forme jj / mm / aaaa
#define NB_CARACTERE_MAX 80 //Nombre de caractères maximal d'une ligne

// Identité d ’ une personne
struct sIdentite
{
int Identifiant ; // Identifiant unique
char * Nom ; // Nom ( cha î ne de caract è res allou é e dynamiquement )
char * Prenom ; // Pr é nom ( cha î ne de caract è res allou é e dynamiquement )
char Sexe ; // ’F ’ ou ’M ’
char DateNaissance [ LG_DATE +1]; // Date de naissance sous la forme jj / mm / aaaa
};  

tIdentite IdentiteCreer(int Id, char *Nom, char *Prenom, char Sexe, char DateNais[])
{
    tIdentite nouvelle_personne;
    nouvelle_personne=malloc(sizeof(struct sIdentite));
    if(nouvelle_personne==NULL) return NULL; //On retourne NULL si l'allocation n'a pas été réalisée correctement
    nouvelle_personne->Identifiant=Id;
    nouvelle_personne->Sexe=Sexe;
    //On copie la chaine de caractères DateNais dans le champs DateNaissance de la sturcture pointée par nouvelle_personne
    for(int i=0; i<(LG_DATE+1); i++) nouvelle_personne->DateNaissance[i]=DateNais[i];
    //On cherche la taille de la chaine de caractères Nom
    int taille_nom=0;
    while(Nom[taille_nom]!='\0') taille_nom++;
    taille_nom++;
    //On copie la chaine de caractères Nom dans le champs Nom de la sturcture pointée par nouvelle_personne
    nouvelle_personne->Nom=malloc(taille_nom*sizeof(char));
    if (nouvelle_personne->Nom==NULL) return NULL;
    for(int i=0; i<taille_nom; i++) nouvelle_personne->Nom[i]=Nom[i];
    //On cherche la taille de la chaine de caractères Prenom
    int taille_prenom=0;
    while(Prenom[taille_prenom]!='\0') taille_prenom++;
    taille_prenom++;
    //On copie la chaine de caractères Prenom dans le champs Prenom de la sturcture pointée par nouvelle_personne
    nouvelle_personne->Prenom=malloc(taille_prenom*sizeof(char));
    if (nouvelle_personne->Prenom==NULL) return NULL;
    for(int i=0; i<taille_prenom; i++) nouvelle_personne->Prenom[i]=Prenom[i];
    return nouvelle_personne;
}

int IdentiteIdentifiant(tIdentite Identite)
{
    return Identite->Identifiant;
}

char *IdentiteNom(tIdentite Identite)
{
    return Identite->Nom;
}

char *IdentitePrenom(tIdentite Identite)
{
    return Identite->Prenom;
}

char IdentiteSexe(tIdentite Identite)
{
    return Identite->Sexe;
}

char *IdentiteDateNaissance(tIdentite Identite)
{
    return Identite->DateNaissance;
}

void IdentiteAfficher(tIdentite Identite)
{
    printf("[%d] %s %s, %c, %s\n", IdentiteIdentifiant(Identite), IdentiteNom(Identite), IdentitePrenom(Identite), IdentiteSexe(Identite), IdentiteDateNaissance(Identite));
}

void IdentiteLiberer(tIdentite Identite)
{
    //On libère les chaines de caractère allouées dynamiquement de la structure pointée par Identite
    free(Identite->Nom);
    free(Identite->Prenom);
    //On libère la structure pointée par Identite
    free(Identite);
}

tIdentite IdentiteLiref(FILE *f)
{
    int Identifiant,nb_lu;
    char Nom[NB_CARACTERE_MAX+1], Prenom[NB_CARACTERE_MAX+1], Sexe, Date_naissance[LG_DATE+1];
    //Lecture de l'Identifiant
    nb_lu=fscanf(f,"%d", &Identifiant);
    if(nb_lu==EOF) return NULL; //On renvoie NULL s'il n'y a plus rien à lire dans le fichier
    //Lecture du Nom
    getc(f); //On récupère '\n'
    int i=0;
    char c=getc(f);
    while(c!=EOF && i<NB_CARACTERE_MAX && c!='\n')
    {
        Nom[i]=c;
        i++;
        c=getc(f);
    }
    Nom[i]='\0';
    if(c==EOF) return NULL;
    //Lecture du Prenom
    i=0;
    c=getc(f);
    while(c!=EOF && i<NB_CARACTERE_MAX && c!='\n')
    {
        Prenom[i]=c;
        i++;
        c=getc(f);
    }
    Prenom[i]='\0';
    if(c==EOF) return NULL;
    //Lecture du Sexe
    nb_lu=fscanf(f,"%c", &Sexe); //On récupère le Sexe de la personne
    if(nb_lu==EOF) return NULL;
    //Lecture de la Date de naissance
    nb_lu=fscanf(f,"%s", Date_naissance);
    if(nb_lu==EOF) return NULL;
    return IdentiteCreer(Identifiant,Nom,Prenom,Sexe,Date_naissance);
}
