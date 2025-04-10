#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "font.h"
#include "timer.h"
#include "affichage.h"
#include "processus.h"

// Fonction de démarrage du noyau
void kernel_start(void)
{   
    // Pour les premières séances 

    // TESTS SEANCE 1
    // Appeler la fonction de test de la séance 1
    // seance1();

    // TESTS SEANCE 2
    // Appeler la fonction de test de la séance 2
    // seance2();

    // TESTS SEANCE 3
    // Appeler la fonction de test à partir de la séance 3
    // seance3();
    // ou
    seance3_ordonnanceur_a_priorite();

    /* On ne doit jamais sortir de kernel_start.
       Cette boucle while(1) maintient le noyau en exécution indéfiniment. */
    while (1) {
        /* Cette fonction arrête le processeur en attendant une interruption.
           Le noyau doit toujours rester actif pour répondre aux interruptions. */
        hlt();
    }
}
