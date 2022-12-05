/*
 TP des listes ENSIAS
*/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

//#define Malloc(type) (type *)malloc(sizeof(type))

#define MAXVILLES 100

#ifdef _WIN32
    #define clear() system("cls");//for windows systems
#else
    #define clear() system("clear");//for *nix systems 
#endif


typedef struct
{
    const char nom[20];
    const char adresse[30];
    int domaine;
} donnees;

typedef struct _adherent
{
    donnees* info;
    struct _adherent* suivant;
} adherent;

typedef adherent* listeAdherents;

typedef struct
{
    char nomVille[25];
    listeAdherents ptrListAdherents;
} Ville;

//**** Fonctions utiles ******
int removeNewLine(char* s) {
    s[strcspn(s,"\n")] = 0;
}

donnees* creerAdherent(char* nom, char* adresse, int domaine) {
    donnees* d = (donnees *)malloc(sizeof(donnees));
    d->domaine = domaine;
    strcpy(d->nom,nom);
    strcpy(d->adresse,adresse);
    return d;
}

donnees* inputDonnees() {
    char nom[20];
    char adresse[30];
    int domaine;
    printf(" Entrer les donnees d'adherent:\n");
    printf("\tNom: ");
    fgets(nom,sizeof(nom),stdin);
    printf("\tAdresse: ");
    fgets(adresse,sizeof(adresse),stdin);
    printf("\tDomaine: ");
    scanf("%d%*c",&domaine);
    removeNewLine(nom);
    removeNewLine(adresse);
    return creerAdherent(nom, adresse, domaine);
}

//** Ex 1 de TP ********
listeAdherents insererAdherent(donnees* d, listeAdherents list, int choix) {
    adherent* nouveau = (adherent *)malloc(sizeof(adherent));
    nouveau->info = d;

    adherent* p = list;
    adherent* pre = list;

    if (p == NULL && 1 <= choix <= 3) {
        nouveau->suivant = list;
        return nouveau;
    }

    switch(choix) {
        case 1: //Ajout au debut de la liste
            nouveau->suivant = list;
            return nouveau;
        case 2: //Ajout au fin de list
            while (p != NULL) {
                pre = p;
                p = p->suivant;
            }
            pre->suivant = nouveau;
            nouveau->suivant = NULL;
            return list;
        case 3: //Ajout par ordre alphabetique
            if (strcmp(p->info->nom, d->nom) > 0) { //superieur
                nouveau->suivant = p;
                return nouveau;
            } else {
                pre = p;
                p = p->suivant;
                while (p != NULL && strcmp(p->info->nom, d->nom) <= 0) {
                    pre = p;
                    p = p->suivant;
                }
            }
            pre->suivant = nouveau;
            nouveau->suivant = p;
            return list;
        default:
            printf("Entrer un choix valid\n");
            return list;
    }
}

int isAdherentEqual(adherent* p, donnees* d) {
    return (strcmp(d->adresse,p->info->adresse) == 0 && strcmp(d->nom, p->info->nom) == 0 && d->domaine == p->info->domaine);
}

void afficherAdherents(listeAdherents list) {
    while (list != NULL) {
        _afficherAdherent(list->info);
        list = list->suivant;
    }
}

void _afficherAdherent(donnees* d) {
    printf("----------  Adherent  ----------\n");
    printf("\tNom: %s\n",d->nom);
    printf("\tAdresse: %s\n",d->adresse);
    printf("\tDomaine: %d\n",d->domaine);
    printf("--------------------------------\n");
}

listeAdherents supprimerAdherent(listeAdherents list, donnees* d) {
    adherent* p = list;
    if (p == NULL) {
        printf("Liste vide\n");
        return p;
    }
    if (isAdherentEqual(p,d) == 1) {
        adherent* q = list;
        p = p->suivant;
        free(q);
        return p;
    }
    while (p->suivant != NULL) {
        if (isAdherentEqual(p->suivant,d) == 1) {
            adherent* q = p->suivant;
            p->suivant = p->suivant->suivant;
            free(q);
            return list;
        }
        p = p->suivant;
    }
    return list;
}

donnees* rechercherAdherent(listeAdherents list, char* nom) {
    while (list != NULL) {
        if (strcmp(list->info->nom, nom) == 0) {
            return list->info;
        }
        list = list->suivant;
    }
    return NULL;
}

//** Ex 2 de TP *********
void afficherAdherentsVille(Ville T[], int n, char *nomV) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(T[i].nomVille,nomV) == 0) {
            break;
        }
    }
    if (i < n) {
        listeAdherents p = T[i].ptrListAdherents;
        int counter = 0;
        while (p != NULL) {
            counter++;
            _afficherAdherent(p->info);
            p = p->suivant;
        }
    } else {
        printf("La ville n'existe pas!\n");
    }
}

void afficherVilles(Ville T[], int n) {
    for (int i = 0; i < n; i++) {
        Ville currentVille = T[i];
        printf("########## Ville: %s ##########\n",currentVille.nomVille);
        printf("\tAdherents:\n");
        afficherAdherents(currentVille.ptrListAdherents);
        printf("###############################\n");
    }
}

int insererAdherentVille(Ville T[], int *n, char* nomV, donnees *d, int choix) {
    int i;
    for (i = 0; i < *n; i++) {//Cherche de la ville dans le tableau
        if (strcmp(T[i].nomVille, nomV) == 0) {
            break;
        }
    }
    if (*n != 0 && i < *n) { //La ville existe
        listeAdherents p = insererAdherent(d, T[i].ptrListAdherents, choix);
        T[i].ptrListAdherents = p;
        return 0; //success
    }
    //La ville n'existe pas
    if (MAXVILLES == *n) { //Le tableau est plein
        return -1; //error
    }

    Ville v = *(Ville *)malloc(sizeof(Ville)); //init Ville
    strcpy(v.nomVille, nomV);
    v.ptrListAdherents = insererAdherent(d, NULL, choix);

    T[*n] = v; //insert ville
    *n += 1;

    return 0; //success
}

int supprimerAdherentVille(Ville T[], int *n, char* nomV, donnees *d) {
    int i = 0;
    for (i = 0; i < *n; i++) {
        if (strcmp(T[i].nomVille,nomV) == 0) {
            break;
        }
    }

    if (i < n) {
        T[i].ptrListAdherents = supprimerAdherent(T[i].ptrListAdherents, d);
        if (T[i].ptrListAdherents == NULL) {
            for (int j = i+1; j < *n; j++) {
                T[j-1] = T[j];
            }
            *n -= 1;
        }
        return 0;
    }
    return -1;
}

donnees* rechercherAdherentVille(Ville T[], int n, char* nom) {
    //on suppose recherche par nom et on s'arrete a la premiere occurence
    //return NULL si aucune occurence n'est trouve
    donnees* a = NULL;
    for (int i = 0; i < n; i++) {
        a = rechercherAdherent(T[i].ptrListAdherents, nom);
        if (a != NULL) {
            return a;
        }
    }
    return a;
}

//** Menu de TP *************
void pressToContinue() {
    printf("Pressez un bouton pour continuer...\n");
    getchar();
}

int getNum() {
    char a;
    scanf(" %c%*c",&a);
    return (int)a - 0x30;
}

void invalidChoice() {
    printf("Choix invalide retour au menu principale!\n");
    pressToContinue();
}

void mainMenu(Ville T[], int* n) {
    clear();
    printf("\n Gestionnaire d'adherents\n");
    printf("\nVeuillez choisir une option:\n\n");
    printf("\t (1) Inserer un adherent\n");
    printf("\t (2) Afficher les adherents\n");
    printf("\t (3) Supprimer un adherent\n");
    printf("\n\t (ESC) Quitter le programme\n\n");
    char input;
    scanf(" %c%*c",&input);
    switch((int)input) {
        case 0x1b:
            clear()
            printf("Vous avez fermer le programme.\nBonne journee ^^\n");
            exit(0);
        case 0x31:
            return subMenuInsert(T, n);
        case 0x32:
            return subMenuAfficher(T, n);
        case 0x33:
            return subMenuDelete(T, n);
        default:
            printf("Choix invalide!\n");
            return mainMenu(T, n);
    }
}

void subMenuDelete(Ville T[], int* n) {
    clear();
    printf("\n Gestionnaire d'adherents (Suppression)\n");
    char nomV[25];
    printf("\nVeuillez entrer la ville en question: ");
    fgets(nomV, 25, stdin);
    removeNewLine(nomV);
    clear();
    donnees* d = inputDonnees();
    if ( supprimerAdherentVille(T, n, nomV, d) == 0 ) {
        printf("Succes!\n");
        pressToContinue();
    } else {
        printf("Echec! Adherent non trouve\n");
        pressToContinue();
    }
    return mainMenu(T, n);
}

void subMenuAfficher(Ville T[], int* n) {
    clear();
    printf("\n Gestionnaire d'adherents (Affichage)\n");
    printf("\nVeuillez choisir une option:\n\n");
    printf("\t (1) Affichage de tous les adherents\n");
    printf("\t (2) Affichage des adherents d'une ville\n");
    printf("\t (0) Retour au menu principal\n");

    switch (getNum()) {
        case 1:
            clear();
            afficherVilles(T, *n);
            pressToContinue();
            return mainMenu(T, n);
        case 2:
            clear();
            char nomV[25];
            printf("Entrer le nom de la ville en question: ");
            fgets(nomV, 25, stdin);
            removeNewLine(nomV);
            afficherAdherentsVille(T, *n, nomV);
            pressToContinue();
            return mainMenu(T, n);
        case 3:
            return mainMenu(T, n);
        default:
            invalidChoice();
            return mainMenu(T, n);
    }
}

void subMenuInsert(Ville T[], int* n) {
    clear();
    printf("\n Gestionnaire d'adherents (Insertion)\n");
    printf("\nVeuillez choisir un type d'insertion.\n\n");
    printf("\t (1) Insertion au debut.\n");
    printf("\t (2) Insertion a la fin.\n");
    printf("\t (3) Insertion par ordre alphabetique.\n");

    int input = getNum();    

    switch(input) {
        case 1:
        case 2:
        case 3:
            clear();
            donnees* d = inputDonnees();
            char nomV[25];
            printf("Entrer le nom de la ville en question: ");
            fgets(nomV, sizeof(nomV), stdin);
            removeNewLine(nomV);
            clear();
            if ( insererAdherentVille(T, n, nomV, d, input) == 0 ) {
                printf("Succes!\n");
                pressToContinue();
                return mainMenu(T, n);
            } else {
                printf("Echec! Tableau est plein.\n");
                pressToContinue();
                return mainMenu(T, n);
            }
        default:
            invalidChoice();
            return mainMenu(T, n);
    }
}

void startMainMenu() {
    Ville tabVille[25];
    int n = 0;
    mainMenu(tabVille, &n);
}

void main() {
   startMainMenu();
}