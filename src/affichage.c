#include "affichage.h"

#include <stdio.h>
#include <string.h>
#include <cpu.h>

// Variables globales pour la position du curseur
int pos_cur_lig = 1;
int pos_cur_col = 0;

// Un exemple de test pour la séance n°1 d'affichage
void seance1() {
    // Positionne le curseur et affiche du texte de test
    place_curseur(getPos_cur_lig(), getPos_cur_col());
    printf("Truc\nSalut\n1\t2\t3\nAB\bC\nBEEF\rRABBIT\n");
    for (int i = 0; i < 82; i++) {
        printf("%d\n", i);
    }
    printf("Hello\n");
    // Décommentez les lignes ci-dessous pour tester le défilement
    // defilement();
    // defilement();
    // defilement();
    // printf("\f");
    // for (int i=0; i<100; i++){
    //     pixel(i, i, 0x00FFFFFF);
    // }
    // ecrit_car(0, 0, 'c');
    // ecrit_car(0, 1, 'o');
    // ecrit_car(0, 2, 'u');
    // ecrit_car(0, 3, 'c');
    // ecrit_car(0, 4, 'o');
    // ecrit_car(0, 5, 'u');

    // for (int s=0; s<3; s++){
    //     efface_ecran();
    //     remplir_ecran();
    // }
    // efface_ecran();
    // ecrit_car(0, 0, 'c');
    // ecrit_car(0, 1, 'o');
    // ecrit_car(0, 2, 'u');
    // ecrit_car(0, 3, 'c');
    // ecrit_car(0, 4, 'o');
    // ecrit_car(0, 5, 'u');
    // place_curseur(0, 1);

    // for (int s=0; s<3; s++){
    //     efface_ecran();
    //     remplir_ecran();
    // }
    // efface_ecran();
    // ecrit_car(0, 0, 'c');
    // ecrit_car(0, 1, 'o');
    // ecrit_car(0, 2, 'u');
    // ecrit_car(0, 3, 'c');
    // ecrit_car(0, 4, 'o');
    // ecrit_car(0, 5, 'u');
    // place_curseur(0, 1);
    // defilement();
}

// Fonction pour dessiner un pixel à une position donnée avec une couleur spécifiée
void pixel(uint32_t x, uint32_t y, uint32_t couleur) {
    uint32_t *ptr;
    ptr = (uint32_t *)(0x80000000 + (x + y * 1280) * 4);
    *ptr = couleur;
}

// Fonction pour obtenir la couleur d'un pixel à une position donnée
uint32_t get_pixel(uint32_t x, uint32_t y) {
    uint32_t *ptr;
    ptr = (uint32_t *)(0x80000000 + (x + y * 1280) * 4);
    return *ptr;
}

// Fonction pour obtenir l'adresse d'un pixel à une position donnée
uint32_t *get_pixel_adress(uint32_t x, uint32_t y) {
    uint32_t *ptr;
    ptr = (uint32_t *)(0x80000000 + (x + y * 1280) * 4);
    return ptr;
}

// Fonction pour écrire un caractère à une position donnée
void ecrit_car(uint32_t lig, uint32_t col, char car) {
    int x0 = col * 8;
    int y0 = lig * 8;
    int bin = 1;
    int val;
    for (int l = 0; l < 8; l++) {
        for (int c = 0; c < 8; c++) {
            // Obtient la valeur du bit correspondant dans le caractère
            val = (font8x8_basic[(int)car][l] & bin) >> c;
            // Dessine le pixel en fonction de la valeur du bit
            pixel(x0 + c, y0 + l, (val & 1) * 0x00FFFFFF);
            bin = bin << 1;
        }
        bin = 1;
    }
}

// Fonction pour remplir l'écran avec une couleur donnée
void remplir_ecran(void) {
    for (int i = 0; i < 1280; i++) {
        for (int j = 0; j < 720; j++) {
            pixel(i, j, 0x00FFFFFF);
        }
    }
}

// Fonction pour effacer l'écran (met tous les pixels à 0)
void efface_ecran(void) {
    for (int i = 0; i < 1280; i++) {
        for (int j = 0; j < 720; j++) {
            pixel(i, j, 0);
        }
    }
}

// Fonction pour placer le curseur à une position spécifiée
void place_curseur(uint32_t lig, uint32_t col) {
    int x0 = col * 8;
    int y0 = lig * 8;
    // Dessine une ligne sous le caractère pour représenter le curseur
    for (int c = 0; c < 8; c++) {
        pixel(x0 + c, y0 + 7, 0x00FFFFFF);
    }
}

// Fonction pour supprimer le curseur à une position spécifiée
void supprime_curseur(uint32_t lig, uint32_t col) {
    int x0 = col * 8;
    int y0 = lig * 8;
    // Supprime la ligne sous le caractère pour effacer le curseur
    for (int c = 0; c < 8; c++) {
        pixel(x0 + c, y0 + 7, 0);
    }
}

// Fonction pour effectuer le défilement de l'écran vers le haut
void defilement(void) {
    if (pos_cur_lig == 1) {
        supprime_curseur(pos_cur_lig, pos_cur_col);
        memmove((void *)(0x80000000 + 1280 * 8 * 4), (void *)(0x80000000 + 1280 * 8 * 4 * 2), (720 - 16) * 1280 * 8);
        pos_cur_col = 0;
        pos_cur_lig = 1;
        place_curseur(pos_cur_lig, pos_cur_col);
    }
    memmove((void *)(0x80000000 + 1280 * 8 * 4), (void *)(0x80000000 + 1280 * 8 * 4 * 2), (720 - 16) * 1280 * 8);
}

// Fonction pour effectuer le défilement et traiter un caractère
void defilement_traite_car(void) {
    memmove((void *)(0x80000000 + 1280 * 8 * 4), (void *)(0x80000000 + 1280 * 8 * 4 * 2), (720 - 16) * 1280 * 8);
}

// Fonction pour traiter un caractère et mettre à jour la position du curseur
void traite_car(char c) {
    int indice = (int)c;
    // Traitement spécial pour la touche de retour arrière
    if (indice == 8) {
        // Vérifie si le curseur n'est pas à la première colonne
        if (pos_cur_col > 0) {
            // Supprime le curseur à la position actuelle
            supprime_curseur(pos_cur_lig, pos_cur_col);
            // Décrémente la position de la colonne du curseur
            pos_cur_col--;
            // Replace le curseur à la nouvelle position
            place_curseur(pos_cur_lig, pos_cur_col);
        }
    }
    // Traitement spécial pour la touche de tabulation
    else if (indice == 9) {
        // Calcule la nouvelle position de la colonne après la tabulation
        int res = ((int)pos_cur_col / 8 + 1) * 8;
        // Vérifie si la nouvelle position de la colonne est dans les limites de l'écran
        if (res < 1280 / 8) {
            supprime_curseur(pos_cur_lig, pos_cur_col);
            pos_cur_col = res;
            place_curseur(pos_cur_lig, pos_cur_col);
        }
        else {
            // Vérifie si le curseur est à la dernière ligne de l'écran
            if (pos_cur_lig == 720 / 8 - 1) {
                supprime_curseur(pos_cur_lig, pos_cur_col);
                defilement_traite_car();
                pos_cur_col = 0;
                place_curseur(pos_cur_lig, pos_cur_col);
            }
            else {
                supprime_curseur(pos_cur_lig, pos_cur_col);
                pos_cur_lig++;
                pos_cur_col = 0;
                place_curseur(pos_cur_lig, pos_cur_col);
            }
        }
    }
    // Traitement spécial pour la touche de saut de ligne
    else if (indice == 10) {
        if (pos_cur_lig < 720 / 8 - 1) {
            supprime_curseur(pos_cur_lig, pos_cur_col);
            pos_cur_lig++;
            pos_cur_col = 0;
            place_curseur(pos_cur_lig, pos_cur_col);
        }
        else {
            supprime_curseur(pos_cur_lig, pos_cur_col);
            defilement_traite_car();
            pos_cur_col = 0;
            place_curseur(pos_cur_lig, pos_cur_col);
        }
    }
    // Traitement spécial pour la touche de retour à la ligne
    else if (indice == 13) {
        supprime_curseur(pos_cur_lig, pos_cur_col);
        pos_cur_col = 0;
        place_curseur(pos_cur_lig, pos_cur_col);
    }
    // Traitement spécial pour la touche de formfeed (effacement d'écran)
    else if (indice == 12) {
        // Efface tout l'écran
        efface_ecran();
        pos_cur_col = 0;
        pos_cur_lig = 1;
        // Replace le curseur à la nouvelle position
        place_curseur(pos_cur_lig, pos_cur_col);
    }
    else {
        supprime_curseur(pos_cur_lig, pos_cur_col);
        // Vérifie si la colonne du curseur est à la fin de l'écran
        if (pos_cur_col == 1280 / 8 - 1) {
            // Vérifie si le curseur est à la dernière ligne de l'écran
            if (pos_cur_lig == 720 / 8 - 1) {
                defilement_traite_car();
                // Réinitialise la position de la colonne
                pos_cur_col = 0;
                ecrit_car(pos_cur_lig, pos_cur_col, c);
                pos_cur_col++;
                place_curseur(pos_cur_lig, pos_cur_col);
            }
            else {
                pos_cur_lig++;
                pos_cur_col = 0;
                ecrit_car(pos_cur_lig, pos_cur_col, c);
                pos_cur_col++;
                place_curseur(pos_cur_lig, pos_cur_col);
            }
        }
        else {
            ecrit_car(pos_cur_lig, pos_cur_col, c);
            pos_cur_col++;
            place_curseur(pos_cur_lig, pos_cur_col);
        }
    }
}

// Fonction pour afficher une chaîne de caractères sur la première ligne
void affichageTime(char *time) {
    for (int i = 10; i > 0; i--) {
        ecrit_car(0, 1280 / 8 - i, time[10 - i]);
    }
}

// Fonction pour traiter une chaîne de caractères
void console_putbytes(const char *s, int len) {
    if (s == NULL) {
        return;
    }
    // Parcours chaque caractère de la chaîne et le traite
    for (int i = 0; i < len; i++) {
        traite_car(s[i]);
    }
}

// Fonction pour obtenir la position actuelle de la ligne du curseur
int getPos_cur_lig() {
    return pos_cur_lig;
}

// Fonction pour obtenir la position actuelle de la colonne du curseur
int getPos_cur_col() {
    return pos_cur_col;
}
