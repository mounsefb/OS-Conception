#ifndef _PROCESSUS_H
#define _PROCESSUS_H

#include <cpu.h>
#include <inttypes.h>

#define STACK_SIZE 4096

typedef enum ETAT{ELU, ACTIVABLE, ENDORMI, MORT} ETAT_T;

// Structure représentant un processus
typedef struct {
    // Propriétés du processus
    int32_t pid;
    char nom[16];
    ETAT_T etat;
    uint64_t sauv[18];
    uint64_t pile[STACK_SIZE];

    // Propriétés spécifiques à l'ordonnancement à priorité
    int cout;
    int periode;
    int indice_priorite;
    uint64_t heure_debut_execution;
    uint64_t cout_consomme;
    uint64_t wakeup;
} process_t;



// Déclaration des fonctions
void seance3(void);
void seance3_ordonnanceur_a_priorite(void);
void init_proc(void);
void tri_processus(int ordonnancement);
void ordonnance(void);
char *mon_nom(void);
int32_t mon_pid(void);
int32_t cree_processus(void (*code)(void), char *nom);
int32_t cree_processus_a_priorite(void (*code)(void), char *nom, int cout, int periode);
int32_t cree_processus_en_attente(void (*code)(void), char *nom);
void dors(uint32_t nbr_secs);
void fin_processus(void);
void afficher_en_haut_a_gauche(char *etat);
void affiche_etats(void);
void proc_launcher(void (*proc)(void));
void idle();
void proc1(void);
void proc2(void);
void proc3(void);
void enAttente(void);

#endif