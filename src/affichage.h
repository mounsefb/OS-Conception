#ifndef _AFFICHAGE_H
#define _AFFICHAGE_H

#include <cpu.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "font.h"

// int pos_cur_lig;
// int pos_cur_col;

void seance1(void);

void pixel(uint32_t x, uint32_t y, uint32_t couleur);
uint32_t get_pixel(uint32_t x, uint32_t y);
uint32_t* get_pixel_adress(uint32_t x, uint32_t y);
void ecrit_car(uint32_t lig, uint32_t col, char car);
void remplir_ecran(void);
void efface_ecran(void);
void place_curseur(uint32_t lig, uint32_t col);
void supprime_curseur(uint32_t lig, uint32_t col);
void defilement(void);
void defilement_traite_car(void);
void traite_car(char c);
void affichageTime(char * time);
void console_putbytes(const char *s, int len);
int getPos_cur_lig();
int getPos_cur_col();


#endif