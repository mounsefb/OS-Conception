# RTOS - Projet Conception Système d'Exploitation

## Introduction

Le RTOS (Real-Time Operating System) est un projet de système d'exploitation temps réel. Ce README fournit une documentation concise sur les choix d'implémentation et les fonctionnalités du système.

## Structure du Projet

Le projet est structuré de manière modulaire pour assurer une gestion claire et maintenable du système. Les composants principaux sont les suivants :

- **processus.h, timer.h, affichage.h (respectivement *.c) :** Fichiers pour les processus, le timer et l'affichage.

## Principaux Choix d'Implémentation

1. **Structuration Modulaire :** Le code est organisé en modules distincts pour faciliter la maintenance et l'extensibilité.

2. **Structures de Données :** Les processus sont représentés par des structures, stockant des informations cruciales pour leur gestion.

3. **Gestion du Temps :** Une approche basée sur le temps est utilisée, avec `nbr_secondes()` pour obtenir le temps écoulé.

4. **Ordonnancement :** Deux politiques sont implémentées - le tourniquet et le Rate Monotonic (RM).

5. **Processus en Attente :** Un type spécial "enAttente" illustre la gestion des processus en attente.

6. **Documentation Détaillée :** Des commentaires dans le code expliquent chaque fonction pour faciliter la compréhension et la maintenance.

## Extension

**Ordonnanceur Rate Monotonic (RM)**
L'extension majeure apportée à ce RTOS est l'implémentation de l'ordonnanceur Rate Monotonic. Cette politique d'ordonnancement assigne les priorités en fonction des périodes des processus. Les processus avec des périodes plus courtes ont une priorité plus élevée.
La structure de donnée correspondante aux processus a donc été étendue pour cette occasion. Les champs cout et priorite servent à stocker les informations consernant le cout et la periode de la tache. Une fois les processus crée, un tri est fait sur le tableau de processus à periode croissante. Le champs indice_priorite sert à contenir l'indice de priorité du processus par rapport aux autres taches. Le pid reste inchangé lors du tri. Finalement, heure_debut_execution et cout_consomme servent à garder une trace de quand le processus à été élu pour la première fois durant sa période courante et dans le cas d'une préemption, quel cout reste-il à consommer.
Le code de l'ordonnancement pourrait être amélioré en évitant par exemple certaines redondances de code.

**Processus en attente**
Un type spécial de processus, appelé "enAttente", a été introduit, il fait office de processus d'attente quand tous les processus "primaires" sont endormis.
