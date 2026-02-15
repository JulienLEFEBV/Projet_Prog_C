#include <stdio.h>
#include <stdlib.h>
#include "identite.h"
#include "genea.h"

// Arbre g é n é alogique
struct sArbre
{
struct sFiche * pPremiere ; // Adresse de la premi è re fiche
struct sFiche * pDerniere ; // Adresse de la derni è re fiche
};
// Fiche associ é e à chaque individu pr é sent dans l ’ arbre
struct sFiche
{
tIdentite Identite ; // Acc è s aux informations de l ’ identit é de la personne
struct sFiche * pPere ; // Adresse de la fiche du p è re
struct sFiche * pMere ; // Adresse de la fiche de la m è re
struct sFiche * pSuivante ; // Adresse de la fiche suivante
};

//-----------------------------------------Niveau2-----------------------------------------------

tArbre ArbreCreer(void)
{
    tArbre Nouv_arbre;
    Nouv_arbre=malloc(sizeof(struct sArbre));
    if (Nouv_arbre==NULL) return NULL;
    //On initialise les pointeurs à NULL
    Nouv_arbre->pPremiere=NULL;
    Nouv_arbre->pDerniere=NULL;
    return Nouv_arbre;
}

void ArbreAfficher(tArbre Arbre)
{
    struct sFiche *Fiche_courante;
    Fiche_courante=Arbre->pPremiere;
    while (Fiche_courante!=NULL)
    {
        //On affiche l'identité de la personne de la fiche courante
        IdentiteAfficher(Fiche_courante->Identite);
        //On affiche l'identité du père de la personne de la fiche courante
        printf("\tPère : ");
        if(Fiche_courante->pPere!=NULL) IdentiteAfficher((Fiche_courante->pPere)->Identite);
        else printf("inconnu\n");
        //On affiche l'identité de la mère de la personne de la fiche courante
        printf("\tMère : ");
        if(Fiche_courante->pMere!=NULL) IdentiteAfficher((Fiche_courante->pMere)->Identite);
        else printf("inconnue\n");
        Fiche_courante=Fiche_courante->pSuivante;
    }   
}

void ArbreAjouterPersonne(tArbre Arbre, tIdentite Identite)
{
    struct sFiche *Nouvelle_fiche;
    Nouvelle_fiche=malloc(sizeof(struct sFiche));
    if(Nouvelle_fiche==NULL) fprintf(stderr,"Problème d'allocation mémoire\n"); //On affiche un message d'erreur si l'allocation ne s'est pas réalisée correctement
    else{
        Nouvelle_fiche->Identite=Identite;
        Nouvelle_fiche->pPere=NULL;
        Nouvelle_fiche->pMere=NULL;
        Nouvelle_fiche->pSuivante=NULL;
        Arbre->pDerniere=Nouvelle_fiche;
        if (Arbre->pPremiere==NULL) Arbre->pPremiere=Nouvelle_fiche; //Si l'arbre est vide on rajoute la personne  au début de l'arbre
        else{
            struct sFiche *Fiche_courante;
            Fiche_courante=Arbre->pPremiere;
            while(Fiche_courante->pSuivante!=NULL) Fiche_courante=Fiche_courante->pSuivante; 
            Fiche_courante->pSuivante=Nouvelle_fiche;
        }
    }
}

void ArbreLiberer(tArbre Arbre)
{
    //On libère l'espace mémoire occupé par les fiches identités
    struct sFiche *Fiche_courante,*Fiche_suivante;
    Fiche_courante=Arbre->pPremiere;
    while(Fiche_courante->pSuivante!=NULL)
    {
        Fiche_suivante=Fiche_courante->pSuivante;
        IdentiteLiberer(Fiche_courante->Identite);
        free(Fiche_courante);
        Fiche_courante=Fiche_suivante;
    }
    IdentiteLiberer(Fiche_courante->Identite);
    free(Fiche_courante);
    //On libère l'espace mémoire occupé par l'arbre
    free(Arbre);
}

tArbre ArbreLirePersonnesFichier(char Fichier[])
{
    FILE *f;
    f=fopen(Fichier,"rt");
    if (f==NULL) //On renvoie NULL si le fichier ne s'est pas overt correctement
    {
        perror(Fichier);
        return NULL;
    } 
    tArbre Nouvel_arbre;
    Nouvel_arbre=ArbreCreer();
    if (Nouvel_arbre==NULL) return NULL; //On revoie NULL si l'arbre ne s'est pas créé correctement
    tIdentite Nouvel_Identite;
    while((Nouvel_Identite=IdentiteLiref(f))!=NULL) ArbreAjouterPersonne(Nouvel_arbre,Nouvel_Identite); //On a déjà intialisé les parents à NULL dans la fonction ArbreAjouterPersonne()
    fclose(f);
    return Nouvel_arbre;
}

void ArbreAjouterLienParente(tArbre Arbre, int IdEnfant, int IdParent, char Parente)
{
    //On cherche d'abord la fiche de l'enfant et celle du parent dans l'arbre
    struct sFiche *Fiche_courante,*Fiche_enfant=NULL,*Fiche_parent=NULL;
    Fiche_courante=Arbre->pPremiere;
    while(Fiche_courante!=NULL)
    {
        if(IdentiteIdentifiant(Fiche_courante->Identite)==IdEnfant) Fiche_enfant=Fiche_courante;
        if(IdentiteIdentifiant(Fiche_courante->Identite)==IdParent) Fiche_parent=Fiche_courante;
        Fiche_courante=Fiche_courante->pSuivante;
    } 
    if (Fiche_enfant==NULL || Fiche_parent==NULL)//On affiche les messages d'erreurs si le parent ou l'enfant n'a pas été trouvé dans l'arbre
    {
        if (Fiche_enfant==NULL) fprintf(stderr,"L'enfant d'identifiant %d n'a pas été trouvé dans l'arbre\n",IdEnfant);
        if (Fiche_parent==NULL) fprintf(stderr,"Le parent d'identifiant %d n'a pas été trouvé dans l'arbre\n",IdParent);
    }
    else
    {
        if(Parente=='M') Fiche_enfant->pMere=Fiche_parent;
        if(Parente=='P') Fiche_enfant->pPere=Fiche_parent;
    }
}

int ArbreLireLienParentef(FILE *f, int *pIdEnfant, int *pIdParent, char *pParente)
{
    int nb_lu;
    nb_lu=fscanf(f,"%d",pIdEnfant);
    if (nb_lu==EOF) return 0; //On retourne 0 s'il n'y a plus rien à lire
    nb_lu=fscanf(f,"%d",pIdParent);
    if (nb_lu==EOF) return 0;
    nb_lu=fscanf(f,"%c",pParente);
    if (nb_lu==EOF) return 0;
    return 1;
}

tArbre ArbreLireLienParenteFichier(tArbre Arbre, char Fichier[])
{
    FILE *f;
    f=fopen(Fichier,"rt");
    if(f==NULL) //On retourne NULL et on affiche un message d'erreur s'il y a un problème lors de l'ouverture du fichier
    {
        perror(Fichier);
        return NULL;
    }
    int IdEnfant,IdParent;
    char Parente;
    while(ArbreLireLienParentef(f,&IdEnfant,&IdParent,&Parente)!=0) ArbreAjouterLienParente(Arbre, IdEnfant, IdParent, Parente);
    fclose(f);
    return Arbre;
}

//-----------------------------------------Niveau3-----------------------------------------------

static void EcrireIdentiteTrieeParSexeGV(tArbre Arbre,FILE* f, char Sexe);

void ArbreEcrireGV(tArbre Arbre, char Fichier[])
{
    FILE *f;
    int verification_ecriture;
    f=fopen(Fichier,"wt");
    if (f==NULL) perror(Fichier); //On affiche un message d'erreur si le fichier ne s'est pas ouvert correctement
    verification_ecriture=fputs("digraph {\n\trankdir = \"BT\" ;\n\n\tnode [shape=box , color=blue , fontname=\"Arial\" , fontsize=10];\n",f);
    if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier); //On affiche un message d'erreur si l'on n'a pas réussit à écrire dans le fichier
    //On écrit tous les noeuds bleus
    EcrireIdentiteTrieeParSexeGV(Arbre,f,'M');
    verification_ecriture=fputs("\n\tnode [color = green];\n",f);
    if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier);
    //On écrit dans le fichier tous les noeuds verts
    EcrireIdentiteTrieeParSexeGV(Arbre,f,'F');
    verification_ecriture=fputs("\n\tedge [dir=none];\n",f);
    if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier); 
    struct sFiche *Fiche_courante;
    Fiche_courante=Arbre->pPremiere;
    //On écrit dans le fichier les liens entre les noeuds
    while(Fiche_courante!=NULL)
    {
        if(Fiche_courante->pPere!=NULL)
        {
            verification_ecriture=fprintf(f,"\t%d -> %d;\n",IdentiteIdentifiant(Fiche_courante->Identite),IdentiteIdentifiant((Fiche_courante->pPere)->Identite));
            if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier);
        }
        if(Fiche_courante->pMere!=NULL)
        {
            verification_ecriture=fprintf(f,"\t%d -> %d;\n",IdentiteIdentifiant(Fiche_courante->Identite),IdentiteIdentifiant((Fiche_courante->pMere)->Identite));
            if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier);
        }
        Fiche_courante=Fiche_courante->pSuivante;
    }
    verification_ecriture=fputc('}',f);
    if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier); 
    fclose(f);
}

static void EcrireIdentiteTrieeParSexeGV(tArbre Arbre,FILE* f, char Sexe)
{
    int verification_ecriture;
    struct sFiche *Fiche_courante;
    Fiche_courante=Arbre->pPremiere;
    while(Fiche_courante!=NULL)
    {
        if(IdentiteSexe(Fiche_courante->Identite)==Sexe)
        {
            verification_ecriture=fprintf(f,"\t%d [label=\"%s\\n%s\\n%s\"]\n",IdentiteIdentifiant(Fiche_courante->Identite),IdentiteNom(Fiche_courante->Identite),IdentitePrenom(Fiche_courante->Identite),IdentiteDateNaissance(Fiche_courante->Identite));
            if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier\n");
        }
        Fiche_courante=Fiche_courante->pSuivante;
    }
}

static void ArbreAfficherAscendantsRec(struct sFiche *Fiche_courante, int niveau);

void ArbreAfficherAscendants(tArbre Arbre, int Identifiant)
{
    struct sFiche *Fiche_courante;
    //On cherche la personne d'identifiant Identifiant dans l'arbre Arbre
    Fiche_courante=Arbre->pPremiere;
    while(Fiche_courante!=NULL && IdentiteIdentifiant(Fiche_courante->Identite)!=Identifiant) Fiche_courante=Fiche_courante->pSuivante;
    if (Fiche_courante==NULL) fprintf(stderr,"La personne d'identifiant %d n'a pas été trouvée dans l'arbre\n", Identifiant); //On affiche un message d'erreur si elle n'a pas été touvée
    else ArbreAfficherAscendantsRec(Fiche_courante, 0); //On appelle la fonction auxiliaire récursive ArbreAfficherAscendantsRec pour afficher les ascendants
}

static void ArbreAfficherAscendantsRec(struct sFiche *Fiche_courante, int niveau)
{
    if (Fiche_courante!=NULL)
    {
        //Affichage
        for(int i=0; i<niveau; i++) printf("\t");//On affiche le nombre de tabulation attendu en fonction du niveau
        if(IdentiteSexe(Fiche_courante->Identite)=='M' && niveau!=0) printf("Père : ");
        if(IdentiteSexe(Fiche_courante->Identite)=='F' && niveau!=0) printf("Mère : ");
        IdentiteAfficher(Fiche_courante->Identite);
        //Appels récursifs
        ArbreAfficherAscendantsRec(Fiche_courante->pPere,niveau+1);
        ArbreAfficherAscendantsRec(Fiche_courante->pMere,niveau+1);
    }
}

static void ArbreEcrireAscendantsGVRec(struct sFiche *Fiche_courante, int Id_descendant, FILE *f);

void ArbreEcrireAscendantsGV(tArbre Arbre, int Identifiant, char Fichier[])
{
    struct sFiche *Fiche_courante;
    //On cherche la personne d'identifiant Identifiant dans l'arbre Arbre
    Fiche_courante=Arbre->pPremiere;
    while(Fiche_courante!=NULL && IdentiteIdentifiant(Fiche_courante->Identite)!=Identifiant) Fiche_courante=Fiche_courante->pSuivante;
    if (Fiche_courante==NULL) fprintf(stderr,"La personne d'identifiant %d n'a pas été trouvée dans l'arbre\n", Identifiant); //On affiche un message d'erreur si elle n'a pas été touvée
    else {
        FILE *f;
        f=fopen(Fichier,"wt");
        if (f==NULL) perror(Fichier);
        else
        {
            int verification_ecriture;
            verification_ecriture=fputs("digraph {\n\trankdir = \"BT\" ;\n\n\tnode [shape=box , color=blue , fontname=\"Arial\" , fontsize=10];\n\n\tedge [dir=none];\n",f);
            if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier); //On affiche un message d'erreur si l'on n'a pas réussit à écrire dans le fichier
            ArbreEcrireAscendantsGVRec(Fiche_courante, IdentiteIdentifiant(Fiche_courante->Identite),f); //On appelle la fonction auxiliaire récursive ArbreAfficherAscendantsGVRec pour afficher les ascendants
            verification_ecriture=fputc('}',f);
            if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier %s\n",Fichier);
            fclose(f);
        }
    }
}

static void ArbreEcrireAscendantsGVRec(struct sFiche *Fiche_courante, int Id_descendant, FILE *f)
{
    if (Fiche_courante!=NULL)
    {
        //Affichage
        int verification_ecriture;
        verification_ecriture=fputc('\n',f);
        if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier\n");
        if(IdentiteSexe(Fiche_courante->Identite)=='M') verification_ecriture=fputs("\tnode [color=blue];\n",f);
        if(IdentiteSexe(Fiche_courante->Identite)=='F') verification_ecriture=fputs("\tnode [color=green];\n",f);
        if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier\n");
        verification_ecriture=fprintf(f,"\t%d [label=\"%s\\n%s\\n%s\"]\n",IdentiteIdentifiant(Fiche_courante->Identite),IdentiteNom(Fiche_courante->Identite),IdentitePrenom(Fiche_courante->Identite),IdentiteDateNaissance(Fiche_courante->Identite));
        if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier\n");
        if(Id_descendant!=IdentiteIdentifiant(Fiche_courante->Identite))
        {
            verification_ecriture=fprintf(f,"\t%d -> %d;\n",Id_descendant,IdentiteIdentifiant(Fiche_courante->Identite));
            if(verification_ecriture==EOF) fprintf(stderr,"Problème lors de l'écriture dans le fichier\n");
        }
        //Appels récursifs
        ArbreEcrireAscendantsGVRec(Fiche_courante->pPere,IdentiteIdentifiant(Fiche_courante->Identite),f);
        ArbreEcrireAscendantsGVRec(Fiche_courante->pMere,IdentiteIdentifiant(Fiche_courante->Identite),f);
    }
}