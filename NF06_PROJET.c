#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct MenuItem {
    char type[10];
    char nom[30];
    float prix;
    int stock;
} MenuItem;

typedef struct Restaurant{
    char nationalite[10];
    int count;
    MenuItem items[15];
} Restaurant;

typedef struct fileAttente {
    char nom[20];
    int num_commande;
    int total_commande;
    struct fileAttente* ptr;
} FA;

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

Restaurant* ecrireRestaurants(FILE *file){
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

int main() {
    FILE *file = fopen("Fichier_Restaurants.csv", "r");
    if (file == NULL) {
        printf("Erreur: Impossible de lire le fichier.\n");
        exit(1);
    }
    Restaurant *ptr=NULL;
    ptr = ecrireRestaurants(file);
    afficherRestaurants(4,ptr);
    fclose(file);
    return 0;
}



