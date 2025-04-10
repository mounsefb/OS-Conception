#include "timer.h"
#include "processus.h"
#include "affichage.h"
#include <stdio.h>

// Variables globales pour le compteur du timer et le compteur de secondes
int compteurTimer = 0;
int compteurSec = 0;

// Fonction de test pour la séance 2
void seance2(void){

    init_traitant_timer(mon_traitant);
    // Active le timer et les interruptions
    enable_timer();
    enable_it();
        // printf("%d", nbr_secondes());

}

// Gestionnaire d'interruption du timer
void trap_handler(uint32_t mcause, uint32_t mie, uint32_t mip){
    // Masque pour traiter les 31 premières valeurs de mcause
    uint32_t masc = 0;
    for (int i = 0; i < 31; i++){
        masc += 1 << i;
    }

    // Vérifie si l'interruption provient du timer (code 7)
    if ((mcause & masc) == 7){
        // Appelle la fonction d'ordonnancement
        ordonnance();
        // Configure la comparaison du timer pour une nouvelle interruption
        *((uint32_t *)CLINT_TIMER_CMP) = *((uint32_t *)CLINT_TIMER) + TIMER_FREQ / IT_FREQ;
        // Incrémente le compteur du timer
        compteurTimer++;

        // Met à jour l'affichage toutes les 20 interruptions du timer (1 seconde)
        if ((compteurTimer % 20) == 0){
            compteurSec++;
            // Crée une chaîne de caractères représentant le temps écoulé
            char time[255];
            sprintf(time, "[%d%d:%d%d:%d%d]", ((compteurSec / (60 * 24)) % 24) / 10, (compteurSec / (60 * 24)) % 10,
                                                ((compteurSec / 60) % 60) / 10, (compteurSec / 60) % 10,
                                                ((compteurSec) % 60) / 10, (compteurSec) % 10);
            // Affiche le temps écoulé sur l'écran
            affichageTime(time);
        }
    }
}

// Fonction pour obtenir le nombre de secondes écoulées
uint32_t nbr_secondes(void){
    return compteurSec;
}

// Initialise le gestionnaire d'interruption du timer
void init_traitant_timer(void (*traitant)(void)){
    // Initialise l'affichage avec un temps initial de [00:00:00]
    char time[255];
    sprintf(time, "[00:00:00]");
    affichageTime(time);
    // Configure le vecteur d'interruption pour utiliser le gestionnaire spécifié
    __asm__("csrw mtvec, %0":: "r"(traitant));
}

// Active le timer
void enable_timer(){
    // Masque pour activer l'interruption du timer
    uint32_t masc = 1 << 7, mie;
    // Lit le registre MIE pour obtenir le masque d'interruption actuel
    __asm__("csrr %0, mie":: "r"(mie));
    // Modifie le masque pour activer l'interruption du timer
    __asm__("csrw mie, %0":: "r"(mie | masc));
    // Configure la comparaison du timer pour générer une interruption à une fréquence spécifique
    *((uint32_t *)CLINT_TIMER_CMP) = *((uint32_t *)CLINT_TIMER) + TIMER_FREQ / IT_FREQ;
}
