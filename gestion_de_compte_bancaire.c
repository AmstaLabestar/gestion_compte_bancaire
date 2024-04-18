#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Définition de la structure client
typedef struct Client {
    char nom[50];
    char prenom[50];
    float montant;
    char cin[20];
} Client;

// Définition de la structure Element
typedef struct Element {
    Client client;
    struct Element *suivant;
} Element;

// Définition de la structure Liste
typedef struct Liste {
    Element *premier;
} Liste;

// Fonction pour effacer la console sur Windows
void effacer_console() {
    system("cls");
}


// Fonction d'initialisation de la liste
Liste *init_liste() {
    Liste *liste = malloc(sizeof(*liste));
    if (liste == NULL) {
        exit(EXIT_FAILURE);
    }
    liste->premier = NULL;
    return liste;
}

// Fonction pour ajouter un élément au début de la liste
Liste *push_front_element_in_list(Liste *liste, Client client) {
    Element *nouvel_element = malloc(sizeof(*nouvel_element));
    if (nouvel_element == NULL) {
        exit(EXIT_FAILURE);
    }
    nouvel_element->client = client;
    nouvel_element->suivant = liste->premier;
    liste->premier = nouvel_element;
    return liste;
}

// Fonction pour afficher un client
void print_Client(Client client) {
    printf("| %-15s | %-15s | %-10.2f | %-9s |\n", client.nom, client.prenom, client.montant, client.cin);
}

// Fonction pour afficher la liste des clients
void print_liste(Liste *liste) {
    printf("-------------------------------------------------------\n");
    printf("|   Nom            |   Prénom       |   Montant   | CIN |\n");
    printf("-------------------------------------------------------\n");

    if(liste == NULL || liste->premier == NULL) {
        printf("La liste est vide.\n");
    } else {
        Element *actuel = liste->premier;
        while(actuel != NULL) {
            print_Client(actuel->client);
            actuel = actuel->suivant;
        }
    }
    printf("-------------------------------------------------------\n");
}

// Fonction pour libérer la mémoire utilisée par la liste
void liberer_liste(Liste *liste) {
    Element *actuel = liste->premier;
    while (actuel != NULL) {
        Element *temp = actuel;
        actuel = actuel->suivant;
        free(temp);
    }
    free(liste);
}

// Fonction pour saisir les informations d'un client
Client saisir_Client() {
    Client client;
    printf("Entrez le nom du client : ");
    fgets(client.nom, sizeof(client.nom), stdin);
    strtok(client.nom, "\n"); // Supprime le '\n' ajouté par fgets
    printf("Entrez le prenom du client : ");
    fgets(client.prenom, sizeof(client.prenom), stdin);
    strtok(client.prenom, "\n"); // Supprime le '\n' ajouté par fgets
    printf("Entrez le montant du client : ");
    scanf("%f", &client.montant);
    getchar(); // Nettoie le tampon d'entrée
    printf("Entrez le CIN  du client : ");
    fgets(client.cin, sizeof(client.cin), stdin);
    strtok(client.cin, "\n"); // Supprime le '\n' ajouté par fgets
    return client;
}

// Fonction pour rechercher un client par cin
Element *rechercher_client_par_cin(Liste *liste, const char *cin) {
    Element *actuel = liste->premier;
    while (actuel != NULL) {
        if (strcmp(actuel->client.cin, cin) == 0) {
            return actuel;
        }
        actuel = actuel->suivant;
    }
    return NULL;
}

// Fonction pour supprimer un client par cin
Liste *supprimer_client_par_cin(Liste *liste, const char *cin) {
    Element *actuel = liste->premier;
    Element *precedent = NULL;
    while (actuel != NULL) {
        if (strcmp(actuel->client.cin, cin) == 0) {
            if (precedent == NULL) {
                liste->premier = actuel->suivant;
            } else {
                precedent->suivant = actuel->suivant;
            }
            free(actuel);
            break;
        }
        precedent = actuel;
        actuel = actuel->suivant;
    }
    return liste;
}



// Fonction pour écrire les informations des clients dans un fichier texte
void ecrire_dans_fichier(Liste *liste, const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    // Déterminer la largeur maximale de chaque champ
    size_t largeur_nom = 0;
    size_t largeur_prenom = 0;
    size_t largeur_cin = 0;
    Element *actuel = liste->premier;
    while (actuel != NULL) {
        size_t longueur_nom = strlen(actuel->client.nom);
        size_t longueur_prenom = strlen(actuel->client.prenom);
        size_t longueur_cin = strlen(actuel->client.cin);
        if (longueur_nom > largeur_nom) largeur_nom = longueur_nom;
        if (longueur_prenom > largeur_prenom) largeur_prenom = longueur_prenom;
        if (longueur_cin > largeur_cin) largeur_cin = longueur_cin;
        actuel = actuel->suivant;
    }

    // Écrire les données formatées dans le fichier
    actuel = liste->premier;
    while (actuel != NULL) {
        fprintf(fichier, "NOM: %-*s\n", (int)largeur_nom, actuel->client.nom);
        fprintf(fichier, "PRENOM: %-*s\n", (int)largeur_prenom, actuel->client.prenom);
        fprintf(fichier, "MONTANT: %.2f\n", actuel->client.montant);
        fprintf(fichier, "CIN: %-*s\n", (int)largeur_cin, actuel->client.cin);
        fprintf(fichier, "-----------------------------------------\n");
        actuel = actuel->suivant;
    }

    fclose(fichier);
    printf("Liste des clients enregistrée dans le fichier '%s'.\n", nom_fichier);
}



//fonction pour retirer de l'argent
void Retrait(Client *client, float Montant){
    if(Montant <= client->montant){
        client->montant -= Montant;
        printf("Retrait de %.2f FCFA effectué avec succès.\n", Montant);
    } else {
        printf("Impossible de retirer %.2f FCFA. Solde insuffisant.\n", Montant);
    }
}


// Fonction pour effectuer un dépôt
void Depot(Client *client, float Montant) {
    client->montant += Montant;
    printf("Dépôt de %.2f MAD effectué avec succès.\n", Montant);
}

// Fonction pour effectuer un virement
void Virement(Client *expediteur, Client *destinataire, float Montant) {
    if (Montant <= expediteur->montant) {
        expediteur->montant -= Montant;
        destinataire->montant += Montant;
        printf("Virement de %.2f MAD effectué avec succès.\n", Montant);
    } else {
        printf("Impossible d'effectuer le virement. Solde insuffisant.\n");
    }
}




// Fonction pour le menu des opérations
void menu_operations(Liste *liste) {
    int choix_operation = 0;
    while (choix_operation != 4) {
        printf("\n------------------ Menu Opérations -----------------\n");
        printf("| 1. Dépôt                                          |\n");
        printf("| 2. Retrait                                        |\n");
        printf("| 3. Virement                                       |\n");
        printf("| 4. Retour au menu principal                       |\n");
        printf("-----------------------------------------------------\n");

        printf("Entrez votre choix : ");
        scanf("%d", &choix_operation);
        getchar(); // Nettoie le tampon d'entrée

        switch (choix_operation) {
            case 1: {
                char cin_depot[20];
                printf("Entrez le CIN du client pour le dépôt : ");
                fgets(cin_depot, sizeof(cin_depot), stdin);
                strtok(cin_depot, "\n"); // Supprime le '\n' ajouté par fgets
                Element *client_depot = rechercher_client_par_cin(liste, cin_depot);

                if (client_depot != NULL) {
                    float Montant;
                    printf("Entrez le montant à déposer : ");
                    scanf("%f", &Montant);
                    getchar(); // Nettoie le tampon d'entrée
                    Depot(&(client_depot->client), Montant);
                } else {
                    printf("Aucun client trouvé avec le CIN : %s\n", cin_depot);
                }
                break;
            }
            case 2: {
                char cin_retrait[20];
                printf("Entrez le CIN du client pour le retrait : ");
                fgets(cin_retrait, sizeof(cin_retrait), stdin);
                strtok(cin_retrait, "\n"); // Supprime le '\n' ajouté par fgets
                Element *client_retrait = rechercher_client_par_cin(liste, cin_retrait);

                if (client_retrait != NULL) {
                    float Montant;
                    printf("Entrez le montant à retirer : ");
                    scanf("%f", &Montant);
                    getchar(); // Nettoie le tampon d'entrée
                    Retrait(&(client_retrait->client), Montant);
                } else {
                    printf("Aucun client trouvé avec le CIN : %s\n", cin_retrait);
                }
                break;
            }
            case 3: {
                char cin_expediteur[20];
                printf("Entrez le CIN de l'expéditeur : ");
                fgets(cin_expediteur, sizeof(cin_expediteur), stdin);
                strtok(cin_expediteur, "\n"); // Supprime le '\n' ajouté par fgets
                Element *expediteur = rechercher_client_par_cin(liste, cin_expediteur);

                if (expediteur != NULL) {
                    char cin_destinataire[20];
                    printf("Entrez le CIN du destinataire : ");
                    fgets(cin_destinataire, sizeof(cin_destinataire), stdin);
                    strtok(cin_destinataire, "\n"); // Supprime le '\n' ajouté par fgets
                    Element *destinataire = rechercher_client_par_cin(liste, cin_destinataire);

                    if (destinataire != NULL) {
                        float Montant;
                        printf("Entrez le montant à transférer : ");
                        scanf("%f", &Montant);
                        getchar(); // Nettoie le tampon d'entrée
                        Virement(&(expediteur->client), &(destinataire->client), Montant);
                    } else {
                        printf("Aucun client trouvé avec le CIN : %s\n", cin_destinataire);
                    }
                } else {
                    printf("Aucun client trouvé avec le CIN : %s\n", cin_expediteur);
                }
                break;
            }
            case 4:
                printf("Retour au menu principal.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    }
}


int main() {
    system("COLOR 2E");
    Liste *liste = init_liste();
    int choix = 0;
    while (choix != 6) {
        printf("\n---------------------- Menu ------------------ \n");
        printf("| 1. Ajouter un client                          |\n");
        printf("| 2. Rechercher un client                       |\n");
        printf("| 3. Supprimer un client                        |\n");
        printf("| 4. Afficher la liste des clients              |\n");
        printf("| 5. Operation                                  |\n");
        printf("| 6. Quitter                                    |\n");
        printf("------------------------------------------------ \n");

        printf("Entrez votre choix : ");
        scanf("%d", &choix);
        getchar(); // Nettoie le tampon d'entrée

        switch (choix) {
            case 1: {
                effacer_console();
                int ajouter_plusieurs = 0;
                do {
                    Client client = saisir_Client();
                    liste = push_front_element_in_list(liste, client);
                    printf("Voulez-vous ajouter un autre client ? (1 pour Oui, 0 pour Non) : ");
                    scanf("%d", &ajouter_plusieurs);
                    getchar(); // Nettoie le tampon d'entrée
                } while (ajouter_plusieurs);
                ecrire_dans_fichier(liste, "Client.txt");
                break;
            }
            case 2: {
                effacer_console();
                char cin_recherche[20];
                printf("Entrez le CIN du client à rechercher : ");
                fgets(cin_recherche, sizeof(cin_recherche), stdin);
                strtok(cin_recherche, "\n"); // Supprime le '\n' ajouté par fgets
                Element *client_recherche = rechercher_client_par_cin(liste, cin_recherche);
                if (client_recherche != NULL) {
                    printf("Client trouvé :\n");
                    print_Client(client_recherche->client);
                } else {
                    printf("Aucun client trouvé avec le CIN : %s\n", cin_recherche);
                }
                break;
            }
            case 3: {
                effacer_console();
                char cin_supprimer[20];
                printf("Entrez le CIN du client à supprimer : ");
                fgets(cin_supprimer, sizeof(cin_supprimer), stdin);
                strtok(cin_supprimer, "\n"); // Supprime le '\n' ajouté par fgets
                liste = supprimer_client_par_cin(liste, cin_supprimer);
                ecrire_dans_fichier(liste, "Client.txt");
                printf("Client supprimé avec succès.\n");
                break;
            }
            case 4:
                effacer_console();
                printf("Liste des clients :\n");
                print_liste(liste);
                break;
            case 5:
                effacer_console();
                menu_operations(liste);
                break;
            case 6:
                effacer_console();
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    }
    liberer_liste(liste);
    return 0;
}
