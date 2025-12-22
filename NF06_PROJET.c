#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Structs -------------------------------------------------------------------------------- */
typedef struct MenuItem {
    char  type[10];
    char  nom[30];
    float prix;
    int   stock;
} MenuItem;

typedef struct Restaurant{
    char     nationalite[10];
    int      count;
    MenuItem items[15];
    float    totalVentes;
} Restaurant;

typedef struct node {
    char         nomRestaurant[20];
    int          numeroCommande;
    float        totalCommande;
    struct node* next;
} node;

/* Fonctions ------------------------------------------------------------------------------ */
void        afficherRestaurants (int nb_restaurants, Restaurant restaurants[4]);
Restaurant* lireRestaurants     (FILE *file);

// Opérations sur la file
node* addNode     (node* head, char nomRestaurant[], int numeroCommande, float totalCommande);
node* removeFirst (node* head);
void  printNode   (node* ptr);
void  printList   (node *head);

/* Main ----------------------------------------------------------------------------------- */
int main() {
    // Recupere les informations des restaurants dans le fichier
    FILE *file = fopen("Fichier_Restaurants.csv", "r");
    if (file == NULL) {
        printf("Erreur: Impossible de lire le fichier.\n");
        exit(1);
    }
    Restaurant* restaurants=NULL;
    restaurants = lireRestaurants(file);
    fclose(file);
    //afficherRestaurants(4, restaurants); /* Pour le debug */

    // Initialisation de la file
    node* head = NULL;

    // Initialisation des statistiques pour l'admin
    int   nombreTotalCommandeServies = 0;
    for (int i=0; i<4; i++) { restaurants[i].totalVentes = 0;}

    // Boucle principale
    bool continuer = true;
    int  choix;
    int choixAdmin;
    while (continuer){
        printf("\n\n======= Selectionnez votre action =======");
        printf(  "\n1 : Profil client"                        );
        printf(  "\n2 : Profil administrateur"                );
        printf(  "\n3 : Quitter"                              );

        printf("\n\nVotre choix : "                           );
        scanf("%d", &choix);
        
        switch (choix) {
        case 1: // Profil client
            /* code pour le profil client
            
            
            
            
            
            
            */
            break;
        
        case 2: // Profil Admin
            printf("\n\n========= Profil Administrateur =========");
            printf(  "\n1 : Servir une commande"                  );
            printf(  "\n2 : Consulter nombre de commande servies" );
            printf(  "\n3 : Consulter le total des ventes"        );

            printf("\n\nVotre choix : "                           );
            scanf("%d", &choixAdmin);

            switch (choixAdmin) {
            case 1: // Servir une commande
                if (head == NULL) { printf("\nPas de commande a servir"); }
                else {
                    nombreTotalCommandeServies++;
                    head = removeFirst(head); // Actualise la head
                    printf("\nCommande servie !");
                }
                break;
            
            case 2: // Nombre commandes
                printf("\n%d commandes servies.", nombreTotalCommandeServies);
                break;
            
            case 3: // Chiffre d'affaires
                for (int i=0; i<4; i++) {
                    printf("\n%s a vendu pour un total de : %.2f euros.", restaurants[i].nationalite, restaurants[i].totalVentes);
                }
            
            default: // Le nombre choisi n'est pas entre 1 et 3
                if (choix<1 && choix>3) { printf("\nChoix invalide T-T"); }
                break;
            }
            break;

        case 3: // Quitter
            printf("\nAu revoir :)");
            continuer = false;
            break;
        
        default: // Le nombre choisi n'est pas entre 1 et 3
            if (choix<1 && choix>3) { printf("\nChoix invalide T-T"); }
            break;
        }
    }


    return 0;
}

/* Implementations fonctions -------------------------------------------------------------- */
/* Operations sur les restaurants */
void afficherRestaurants(int nb_restaurants, Restaurant restaurants[4]){
    // AFFICHAGE
    for (int i = 0; i < nb_restaurants; i++) {
        printf("Restaurant %s\n", restaurants[i].nationalite);
        printf("Nombre d'items dans le menu : %d\n", restaurants[i].count);
        for (int j = 0; j < restaurants[i].count; j++) {
            MenuItem *it = &restaurants[i].items[j];
            printf("  - %s (%s) : %.2f (stock %d)\n",
                   it->nom, it->type, it->prix, it->stock);
        }
        printf("\n");
    }
}

Restaurant* lireRestaurants(FILE *file){
    Restaurant *restaurants = (Restaurant *)malloc(4 * sizeof(Restaurant));
    if (restaurants == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    int nb_restaurants = 0;
    int current = -1;   // restaurant courant

    char ligne[1000];

    while (fgets(ligne, sizeof(ligne), file)) {

        // supprimer le \n
        ligne[strcspn(ligne, "\n")] = 0;

        // ignorer séparateur
        if (strcmp(ligne, ";;;") == 0 || strlen(ligne) == 0) {
            continue;
        }

        // nouveau restaurant
        if (strstr(ligne, ";;;")) {
            ligne[strcspn(ligne, ";")] = 0;
            current = nb_restaurants;
            strcpy(restaurants[current].nationalite, ligne);
            restaurants[current].count = 0;
            nb_restaurants++;
            continue;
        }

        // sinon : c'est un plat
        if (current != -1 && restaurants[current].count <15) {
            MenuItem *item = &restaurants[current].items[restaurants[current].count];
            sscanf(ligne, "%9[^;];%29[^;];%f;%d", item->type, item->nom, &item->prix, &item->stock);
            restaurants[current].count++;
        }
    }

    for (int i=0;i<4;i++){

    }
    return restaurants;
}

/* Operations sur la file */
// Ajoute un node en bout de file
node* addNode(node* head, char nomRestaurant[], int numeroCommande, float totalCommande) {
    // Cherche le bout de la file
    node* ptr = head;
    while (ptr != NULL) {
        ptr = ptr->next;
    }
    
    // Cree le node
    node* pNew;
    pNew = (node*)malloc(sizeof(node));
    if (pNew == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    strcpy(pNew->nomRestaurant, nomRestaurant);
    pNew->numeroCommande      = numeroCommande;
    pNew->totalCommande       = totalCommande;
    pNew->next                = NULL;

    // Ajoute le node a la file (devient la head si head est null)
    if (head == NULL) { head = pNew; }
    else { ptr->next = pNew; }

    return head;
}

// Retire le 1er node
node* removeFirst(node* head) {
    if (head == NULL) {
        printf("Impossible de retirer le premier noeud car head est null !");
        return NULL;
    }

    // Le node suivant devient la head
    else {
        node* ptr = head;
        head      = head->next;
        free(ptr);
        return head;
    }
}

// Affichages
void printNode(node* ptr){
    if (ptr = NULL) { printf("Le noeud n'existe pas !"); }
    else {
        printf("Restaurant : %s"   , ptr->nomRestaurant );
        printf("\nCommande n° : %d", ptr->numeroCommande);
        printf("\nTotal : %.2f"    , ptr->totalCommande , " euros");
    }
}
void printList(node *head){
    node* ptr = head;
    if (head == NULL) {
        printf("La file est vide !");
        return;
    }
    while (ptr != NULL) {
        printNode(ptr);
        ptr = ptr->next;
    }
}
