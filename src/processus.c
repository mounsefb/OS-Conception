#include "processus.h"
#include "timer.h"
#include "affichage.h"

#define NBRE_PROCESS 10
#include <stdio.h>
#include <string.h>

// Gestion de l'ordonnanceur et choix de conception
// -------------------------------------------------
// Ce programme implémente un ordonnanceur simple prenant en charge deux types d'ordonnancement : tourniquet et Rate Monotonic (RM).
// Les processus sont stockés dans un tableau, et l'ordonnancement est réalisé en fonction du choix de l'utilisateur.
// L'ordonnanceur RM trie les processus par ordre croissant de période.
// La fonction enAttente est utilisée dans le cas où tous les processus sont endormis.
// Dans cette situation, elle permet de maintenir le système en attente jusqu'à ce qu'au moins un processus devienne actif,
// évitant ainsi une utilisation inutile du processeur lorsqu'aucun processus n'est prêt à s'exécuter.
// -------------------------------------------------

process_t table_process[NBRE_PROCESS];  // Tableau des processus
process_t *actif;  // Processus actif
uint32_t nbreProcessActuel;  // Nombre de processus actuels
int ordonnancement_utilisateur = 0;  // Type d'ordonnancement
process_t tmp_process;  // Processus temporaire pour le tri

// Fonction de test principale à partir de la 3eme seance
void seance3(void) {
    seance2();

    init_proc();
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    printf("pid du proc cree = %d\n", cree_processus(&proc1, "proc1"));
    printf("pid du proc cree = %d\n", cree_processus(&proc2, "proc2"));
    printf("pid du proc cree = %d\n", cree_processus(&proc3, "proc3"));
    tri_processus(0);
    idle();
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
}

// Fonction de test à partir de la 3eme seance avec ordonnanceur à priorité
void seance3_ordonnanceur_a_priorite(void) {
    seance2();
    init_proc();

    // -------------------------------------------------


    // Un exemple de test qui met en valeur, la "non-ordonnaçabilité" des taches, 
    // et la préemption d'un processus par un autre plus prioritaire

    table_process[0].cout = 1;
    table_process[0].periode = 6;
    nbreProcessActuel = 1;
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    printf("pid du proc cree = %d\n", cree_processus_a_priorite(&proc1, "proc1", 2, 8));
    printf("pid du proc cree = %d\n", cree_processus_a_priorite(&proc2, "proc2", 1, 2));
    printf("pid du proc cree = %d\n", cree_processus_a_priorite(&proc2, "proc3", 3, 10));
    printf("pid du proc cree = %d\n", cree_processus_en_attente(&enAttente, "enAttente"));

    // -------------------------------------------------

    // Un exemple de test qui met en valeur le cas où le processeur n'est pas utilisé à 100% 
    // On fait alors appel à un autre processus d'attente en attendant qu'une tâche demande à être réalisée
    
    // table_process[0].cout = 1;
    // table_process[0].periode = 6;
    // nbreProcessActuel = 1;
    // printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    // printf("pid du proc cree = %d\n", cree_processus_a_priorite(&proc1, "proc1", 1, 6));
    // printf("pid du proc cree = %d\n", cree_processus_a_priorite(&proc2, "proc2", 1, 6));
    // printf("pid du proc cree = %d\n", cree_processus_a_priorite(&proc2, "proc3", 1, 6));
    // printf("pid du proc cree = %d\n", cree_processus_en_attente(&enAttente, "enAttente"));

    // -------------------------------------------------

    tri_processus(1);
    idle();
    actif = &table_process[0];
}

// Initialisation du premier processus idle
void init_proc() {
    table_process[nbreProcessActuel].pid = nbreProcessActuel;
    sprintf(table_process[nbreProcessActuel].nom, "idle");
    table_process[nbreProcessActuel].etat = ACTIVABLE;

    table_process[nbreProcessActuel].sauv[1] = (uint64_t)&table_process[nbreProcessActuel].pile[STACK_SIZE];
    table_process[nbreProcessActuel].sauv[0] = proc_launcher;
    table_process[nbreProcessActuel].sauv[17] = (uint64_t)idle;

    nbreProcessActuel = 1;

    actif = &table_process[0];
}

// Création d'un processus standard
int32_t cree_processus(void (*code)(void), char *nom) {
    if (nbreProcessActuel >= NBRE_PROCESS) return -1;
    table_process[nbreProcessActuel].pid = nbreProcessActuel;
    sprintf(table_process[nbreProcessActuel].nom, nom);
    table_process[nbreProcessActuel].etat = ACTIVABLE;

    table_process[nbreProcessActuel].sauv[1] = (uint64_t)&table_process[nbreProcessActuel].pile[STACK_SIZE];
    table_process[nbreProcessActuel].sauv[0] = proc_launcher;
    table_process[nbreProcessActuel].sauv[17] = (uint64_t)code;
    nbreProcessActuel++;
    return nbreProcessActuel - 1;
}

// Création d'un processus avec ordonnancement à priorité
int32_t cree_processus_a_priorite(void (*code)(void), char *nom, int cout, int periode) {
    if (nbreProcessActuel >= NBRE_PROCESS) return -1;
    table_process[nbreProcessActuel].pid = nbreProcessActuel;
    sprintf(table_process[nbreProcessActuel].nom, nom);
    table_process[nbreProcessActuel].etat = ACTIVABLE;

    table_process[nbreProcessActuel].cout = cout;
    table_process[nbreProcessActuel].periode = periode;

    table_process[nbreProcessActuel].sauv[1] = (uint64_t)&table_process[nbreProcessActuel].pile[STACK_SIZE];
    table_process[nbreProcessActuel].sauv[0] = proc_launcher;
    table_process[nbreProcessActuel].sauv[17] = (uint64_t)code;
    nbreProcessActuel++;
    return nbreProcessActuel - 1;
}

// Création d'un processus en attente
int32_t cree_processus_en_attente(void (*code)(void), char *nom) {
    if (nbreProcessActuel >= NBRE_PROCESS) return -1;
    table_process[nbreProcessActuel].pid = nbreProcessActuel;
    sprintf(table_process[nbreProcessActuel].nom, nom);
    table_process[nbreProcessActuel].etat = ACTIVABLE;
    table_process[nbreProcessActuel].indice_priorite = nbreProcessActuel;

    table_process[nbreProcessActuel].sauv[1] = (uint64_t)&table_process[nbreProcessActuel].pile[STACK_SIZE];
    table_process[nbreProcessActuel].sauv[0] = proc_launcher;
    table_process[nbreProcessActuel].sauv[17] = (uint64_t)code;
    return nbreProcessActuel;
}

// Tri des processus selon le type d'ordonnancement
void tri_processus(int ordonnancement) {
    ordonnancement_utilisateur = ordonnancement;
    if (ordonnancement_utilisateur == 0) {
        // Cas tourniquet
        printf("Oooooh tourniquet\n");
    } else if (ordonnancement_utilisateur != 0) {
        // Tri des processus par ordre croissant de période (tri par insertion lent mais le plus simple à implémenter)
        if (ordonnancement_utilisateur == 1) {
            // Cas Rate Monotonic
            printf("\n");
            printf("Oooooh raate monotoniiiiic\n");
            printf("\n");

            int indice_min = 0;
            for (uint32_t i = 0; i < nbreProcessActuel - 1; i++) {
                indice_min = i;
                for (uint32_t j = i + 1; j < nbreProcessActuel; j++) {
                    if (table_process[j].periode < table_process[indice_min].periode) {
                        indice_min = j;
                    }
                }

                // Échange de processus
                memmove(&tmp_process, &table_process[i], sizeof(process_t));
                memmove(&table_process[i], &table_process[indice_min], sizeof(process_t));
                memmove(&table_process[indice_min], &tmp_process, sizeof(process_t));
            }

            printf("Resultat du tri des processus par ordre croissant de periode :\n");
            for (uint32_t i = 0; i < nbreProcessActuel; i++) {
                table_process[i].indice_priorite = i;
                printf("%s, ", table_process[i].nom);
            }
            printf("\n");
            actif = &table_process[0];
            actif->etat = ELU;
            actif->heure_debut_execution = 0;
        }
    }
}

// Ordonnanceur
void ordonnance(void) {
    switch (ordonnancement_utilisateur) {
        case 0:
            if (nbreProcessActuel == 1 || nbreProcessActuel == 0) return;
            if (actif->etat == ELU) {
                actif->etat = ACTIVABLE;
            }
            int tmp = actif->pid;
            actif = &table_process[(actif->pid + 1) % nbreProcessActuel];
            do {
                if (actif->etat == ENDORMI) {
                    if (actif->wakeup == nbr_secondes()) {
                        actif->etat = ACTIVABLE;
                    } else {
                        actif = &table_process[(actif->pid + 1) % nbreProcessActuel];
                    }
                }
                if (actif->etat == MORT) {
                    actif = &table_process[(actif->pid + 1) % nbreProcessActuel];
                }
            } while (actif->etat != ACTIVABLE);
            actif->etat = ELU;
            affiche_etats();
            ctx_sw(table_process[tmp].sauv, table_process[actif->pid].sauv);
            break;
        case 1:
            tmp = actif->indice_priorite;
            if (nbreProcessActuel == 1 || nbreProcessActuel == 0) return;
            affiche_etats();
            int endormi = 0;
            for (int i = 0; i < nbreProcessActuel; i++) {
                if (table_process[i].etat == ENDORMI) {
                    endormi++;
                }
                if (table_process[i].etat == ENDORMI && table_process[i].wakeup == nbr_secondes()) {
                    table_process[i].etat = ACTIVABLE;
                }
            }
            if (endormi == nbreProcessActuel && actif->indice_priorite != nbreProcessActuel) {
                printf("Chhhuuuuuuut !! Tous les processus dorment !\n");
                actif = &table_process[nbreProcessActuel];
                ctx_sw(table_process[tmp].sauv, table_process[nbreProcessActuel].sauv);
            }
            if (actif->etat == ELU && (actif->heure_debut_execution + actif->cout == nbr_secondes() || actif->cout_consomme == actif->cout)) {
                printf("\nje m'endors pendant %d secondes \n\n", actif->periode - nbr_secondes() % actif->periode);
                if (nbr_secondes() % actif->periode == 0) {
                    actif->etat = ACTIVABLE;
                } else {
                    dors(actif->periode - nbr_secondes() % actif->periode);
                }
            } else if (actif->etat == ELU) {
                uint32_t i = actif->indice_priorite, j = 0;
                while (j < i && table_process[j].etat != ACTIVABLE) {
                    if (table_process[j].etat == ENDORMI && table_process[j].wakeup == nbr_secondes()) {
                        table_process[j].etat = ACTIVABLE;
                        break;
                    }
                    j++;
                }
                if (j != i) {
                    actif->cout_consomme += nbr_secondes() - actif->heure_debut_execution;
                    actif->etat = ACTIVABLE;
                    actif = &table_process[j];
                    actif->etat = ELU;
                    actif->heure_debut_execution = nbr_secondes();
                    actif->cout_consomme = 0;
                } else {
                    actif->cout_consomme += nbr_secondes() - actif->heure_debut_execution;
                }
            } else if (actif->etat == ENDORMI) {
                uint32_t i = actif->indice_priorite, j = 0;
                while (j < nbreProcessActuel && table_process[j].etat != ACTIVABLE) {
                    if (table_process[(i + j) % nbreProcessActuel].etat == ENDORMI && table_process[(i + j) % nbreProcessActuel].wakeup == nbr_secondes()) {
                        table_process[(i + j) % nbreProcessActuel].etat = ACTIVABLE;
                        break;
                    }
                    j++;
                }
                if (j == nbreProcessActuel) return;
                actif = &table_process[j];
                actif->etat = ELU;
                actif->heure_debut_execution = nbr_secondes();
                actif->cout_consomme = 0;
            } else if (actif->etat == ACTIVABLE) {
                int i = 0, j = actif->indice_priorite;
                while (i < nbreProcessActuel && table_process[i].etat != ACTIVABLE) {
                    i++;
                }
                if (j != i) {
                    actif = &table_process[i];
                    actif->etat = ELU;
                    actif->heure_debut_execution = nbr_secondes();
                    actif->cout_consomme = 0;
                }
            }
            ctx_sw(table_process[tmp].sauv, table_process[actif->indice_priorite].sauv);
            break;
    }
}

// Renvoie le nom du processus actif
char *mon_nom(void) {
    return actif->nom;
}

// Renvoie le PID du processus actif
int32_t mon_pid(void) {
    return actif->pid;
}

// Met le processus actif en état ENDORMI pour un certain nombre de secondes
void dors(uint32_t nbr_secs) {
    actif->etat = ENDORMI;
    actif->wakeup = nbr_secondes() + nbr_secs;
    ordonnance();
}

// Termine le processus actif
void fin_processus(void) {
    actif->etat = MORT;
    printf("[%s] je suis mort\n", mon_nom());
    ordonnance();
}

// Affiche les états des processus en haut à gauche de l'écran
void afficher_en_haut_a_gauche(char *etat) {
    for (size_t i = 0; i < strlen(etat); i++) {
        ecrit_car(0, i, etat[i]);
    }
}

// Affiche les états de tous les processus
void affiche_etats(void) {
    char stretat[256];
    int taille = 0;
    for (size_t i = 0; i < nbreProcessActuel; i++) {
        if (i == 0) {
            sprintf(stretat, "Processus %s :", table_process[i].nom);
        } else {
            char process[24]; // Ajustez la taille en fonction de vos besoins
            sprintf(process, "Processus %s :", table_process[i].nom);
            strcat(stretat, process);
        }
        taille += 16;
        if (table_process[i].etat == ELU) {
            strcat(stretat, " ELU       ");
        } else if (table_process[i].etat == ACTIVABLE) {
            strcat(stretat, " ACTIVABLE ");
        } else if (table_process[i].etat == ENDORMI) {
            strcat(stretat, " ENDORMI   ");
        } else if (table_process[i].etat == MORT) {
            strcat(stretat, " MORT      ");
        }
        taille += 11;
    }
    afficher_en_haut_a_gauche(stretat);
}

// Fonction de lancement du processus
void proc_launcher(void (*proc)(void)) {
    proc();
    fin_processus();
}

// FONCTION DE PROCESSUS POUR TESTER LA TERMINAISON IMPLICITE ET L'ENDORMICEMENT
// void idle()
// {
//     for (;;) {
//         printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//         enable_it();
//         hlt();
//         disable_it();
//     }
// }

// void proc1(void)
// {
//     for (int32_t i = 0; i < 2; i++) {
//         printf("[temps = %u] processus %s pid = %i\n",
//                nbr_secondes(),
//                mon_nom(),
//                mon_pid());
//         dors(2);
//     }
// }

// void proc2(void)
// {
//     for (;;) {
//         printf("[temps = %u] processus %s pid = %i\n",
//                nbr_secondes(),
//                mon_nom(),
//                mon_pid());
//         dors(1);
//     }
// }

// void proc3(void)
// {
//     for (int i = 0;i < 2; i++) {
//         printf("[temps = %u] processus %s pid = %i\n",
//                nbr_secondes(),
//                mon_nom(),
//                mon_pid());
//         dors(5);
//     }
// }

// FONCTION DE PROCESSUS POUR TESTER L'ORDONNACEMENT À PRIORITE

// Processus "idle"
void idle() {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        enable_it();
        hlt();
        disable_it();
    }
}

// Exemple de processus
void proc1(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        enable_it();
        hlt();
        disable_it();
    }
}

// Exemple de processus
void proc2(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        enable_it();
        hlt();
        disable_it();
    }
}

// Exemple de processus
void proc3(void) {
    for (;;) {
        printf("[temps = %u] processus %s pid = %i\n",
               nbr_secondes(),
               mon_nom(),
               mon_pid());
        enable_it();
        hlt();
        disable_it();
    }
}

// Exemple de processus en attente (attente infinie)
void enAttente(void) {
    for (;;) {
        enable_it();
        hlt();
        disable_it();
    }
}



