#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Structs ------------------------------------------------------------------------------------------------------------------------------------------ */
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
    char         nomRestaurant[10];
    int          numeroCommande;
    float        totalCommande;
    struct node* next;
} node;

/* Fonctions ---------------------------------------------------------------------------------------------------------------------------------------- */
void        afficherRestaurants (int nb_restaurants, Restaurant restaurants[4]);
Restaurant* lireRestaurants     (FILE *file);

// Operations sur la file
node* addNode     (node* head, char nomRestaurant[], int numeroCommande, float totalCommande); // enqueue()
node* removeFirst (node* head);                                                                // dequeue()
bool  isEmpty     (node* head);
void  printNode   (node* ptr);
void  printList   (node *head);

/* Main --------------------------------------------------------------------------------------------------------------------------------------------- */
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

    // Initialisation pour les clients
    int numeroCommande = 0;

    // Initialisation pour l'admin
    int nombreTotalCommandeServies = 0;
    for (int i=0; i<4; i++) { restaurants[i].totalVentes = 0;}

    // Boucle principale
    bool continuer = true;
    int  choix;
    while (continuer){
        printf("\n\n======= Selectionnez votre action =======");
        printf(  "\n1 : Profil client"                        );
        printf(  "\n2 : Profil administrateur"                );
        printf(  "\n3 : Quitter"                              );

        printf("\n\nVotre choix : "                           );
        scanf(" %d", &choix);
        
        switch (choix) {
        case 1: // Profil client
            printf("\n============= Profil Client =============");
            printf("\nListe des restaurants :"                  );
            for (int i=0; i<4; i++) { 
            printf("\n%d : %s", i+1, restaurants[i].nationalite ); }
            
            printf("\n\nVotre choix : "                         );
            int choixClientRestaurant;
            scanf(" %d", &choixClientRestaurant);
            if (choixClientRestaurant <= 0 || choixClientRestaurant > 4) {
                printf("\nChoix invalide T-T");
                break;
            }
            choixClientRestaurant--; // Realigne le choix avec l'index

            // Affiche le menu du restaurant selectionne et permet de faire sa commande
            MenuItem commande[20];
            int      indexCommande        = 0;
            float    total                = 0;
            char     choixClientContinuer = 'o';
            while (choixClientContinuer == 'o') {
                printf("\nMenu :");
                for (int i=0; i<restaurants[choixClientRestaurant].count; i++) {
                printf(  "\n%d : %s (%s)", i+1, restaurants[choixClientRestaurant].items[i].nom, restaurants[choixClientRestaurant].items[i].type); }

                printf("\n\nVotre choix : ");
                int choixClientPlat;
                scanf(" %d", &choixClientPlat);
                if (choixClientPlat <= 0 || choixClientPlat > restaurants[choixClientRestaurant].count) {
                    printf("\nChoix invalide T-T");
                    continue;
                }
                choixClientPlat--; // Realigne le choix avec l'index

                // Regarde si les stocks sont suffisants sachant qu'on n'actualise les stocks qu'après la validation de la commande
                int stockMinimumRequis = 1;
                for (int i=0; i<indexCommande; i++) { 
                    if (strcmp(restaurants[choixClientRestaurant].items[choixClientPlat].nom, commande[i].nom) == 0) { stockMinimumRequis++; } 
                }
                // Ajoute le plat a la commande si disponible
                if (restaurants[choixClientRestaurant].items[choixClientPlat].stock >= stockMinimumRequis || restaurants[choixClientRestaurant].items[choixClientPlat].stock == -1 /* stocks illimites */) {
                    commande[indexCommande] = restaurants[choixClientRestaurant].items[choixClientPlat];
                    total += restaurants[choixClientRestaurant].items[choixClientPlat].prix;
                    indexCommande++;
                } else { printf("\nPlus de stock ^^'"); }
                
                printf("\nTotal : %.2f euros", total);
                printf("\nSouhaitez-vous autre chose avec ceci ? (o/n) : ");
                scanf(" %c", &choixClientContinuer);
            }
            // Affiche la commande
            printf(  "\nCommande :"                         );
            for (int i=0; i<indexCommande; i++) {
            printf("\n%s (%s) : %.2f euros", commande[i].nom, commande[i].type, commande[i].prix); }
            
            printf("\n\nTotal : %.2f euros"          , total);
            printf(  "\nValidez-vous la commande ? (o/n) : ");
            char choixClientValider;
            scanf(" %c", &choixClientValider);
            // Si la commande est validee, on actualise les stocks et on cree le node
            if (choixClientValider == 'o' && total != 0) {
                // Actualise les stocks
                for (int i = 0; i < indexCommande; i++) {
                    for (int j = 0; j < restaurants[choixClientRestaurant].count; j++) {
                        if (strcmp(restaurants[choixClientRestaurant].items[j].nom, commande[i].nom) == 0) {
                            if (restaurants[choixClientRestaurant].items[j].stock > 0 /* Securite sur le stock */) {
                                restaurants[choixClientRestaurant].items[j].stock--;
                            } else if (restaurants[choixClientRestaurant].items[j].stock == -1) {
                                // On ne fais rien car stock illimite
                            } else { printf("\nErreur ! Stock insuffisant pour %s", commande[i].nom); }
                        break; // Sort de la boucle une fois le plat trouvé
                        }
                    }
                }

                // Ajoute le node
                head = addNode(head, restaurants[choixClientRestaurant].nationalite, numeroCommande, total);
                numeroCommande++; // Incremente pour la prochaine commande
            } else {
                printf("\n\nCommande annulee");
            }
            break;
        
        case 2: // Profil Admin
            printf("\n\n========= Profil Administrateur =========");
            printf(  "\n1 : Servir une commande"                  );
            printf(  "\n2 : Consulter nombre de commande servies" );
            printf(  "\n3 : Consulter le total des ventes"        );

            printf("\n\nVotre choix : "                           );
            int choixAdmin;
            scanf(" %d", &choixAdmin);

            switch (choixAdmin) {
            case 1: // Servir une commande
                if (head == NULL) { printf("\nPas de commande a servir"); }
                else {
                    // Cherche a quel restaurant attribuer la vente
                    for (int i=0; i<4; i++) {
                        if (strcmp(head->nomRestaurant, restaurants[i].nationalite) == 0) {
                            restaurants[i].totalVentes += head->totalCommande;
                            break; // Sort une fois le restaurant trouve
                        }
                    }
                    printf("\nCommande %d servie pour %.2f euros !", head->numeroCommande, head->totalCommande);
                    head = removeFirst(head); // Actualise la head
                    nombreTotalCommandeServies++;
                }
                break;
            
            case 2: // Nombre commandes
                printf("\n%d commandes servies.", nombreTotalCommandeServies);
                break;
            
            case 3: // Chiffre d'affaires
                for (int i=0; i<4; i++) {
                    printf("\n%s a vendu pour un total de : %.2f euros.", restaurants[i].nationalite, restaurants[i].totalVentes);
                }
                break;
            
            default: // Le nombre choisi n'est pas entre 1 et 3
                printf("\nChoix invalide T-T");
                break;
            }
            break;

        case 3: // Quitter
            printf("Au revoir :)\n\n");
            continuer = false;
            break;
        
        default: // Le nombre choisi n'est pas entre 1 et 3
            printf("\nChoix invalide T-T");
            break;
        }
    }

    // Libere la memoire avant de fermer le programme
    freeList(head);
    return 0;
}

/* Implementations fonctions ------------------------------------------------------------------------------------------------------------------------ */
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
        printf("\nMemory allocation failed\n");
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

    return restaurants;
}

/* Operations sur la file */
// Ajoute un node en bout de file
node* addNode(node* head, char nomRestaurant[], int numeroCommande, float totalCommande) {
    // Cree le node
    node* pNew;
    pNew = (node*)malloc(sizeof(node));
    if (pNew == NULL) {
        printf("\nMemory allocation failed!");
        exit(1);
    }
    strcpy(pNew->nomRestaurant, nomRestaurant);
    pNew->numeroCommande      = numeroCommande;
    pNew->totalCommande       = totalCommande;
    pNew->next                = NULL;

    // Devient la head si head est null)
    if (head == NULL) { return pNew; }
    
    // Cherche le bout de la file
    node* ptr = head;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    
    // Ajoute le node a la file
    ptr->next = pNew;
    return head;
}

// Retire le 1er node
node* removeFirst(node* head) {
    if (head == NULL) {
        printf("\nImpossible de retirer le premier noeud car head est null !");
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

bool isEmpty (node* head) {
    if (head == NULL) { return true; }
    return false;
}

// Libere la memoire de la liste
void freeList(node* head) {
    node* current = head;
    while (current != NULL) {
        node* next = current->next;
        free(current);
        current = next;
    }
}

// Affichages
void printNode(node* ptr){
    if (ptr = NULL) { printf("\nLe noeud n'existe pas !"); }
    else {
        printf("\nRestaurant : %s"  , ptr->nomRestaurant );
        printf("\nCommande n° : %d" , ptr->numeroCommande);
        printf("\nTotal : %.2f"     , ptr->totalCommande , " euros");
    }
}
void printList(node *head){
    node* ptr = head;
    if (head == NULL) {
        printf("\nLa file est vide !");
        return;
    }
    while (ptr != NULL) {
        printNode(ptr);
        ptr = ptr->next;
    }
}
