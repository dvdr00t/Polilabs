//
//  f_ric.h
//  prova
//
//  Created by Davide Arcolini on 24/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef __DIAG__H
#define __DIAG__H
#define ELEMENTI 5
#include "elementi.h"

typedef struct diag {
    int diagonale[ELEMENTI];
    int cnt_elementi;
    int valore;
} diag_t;

diag_t diagonale_init(void);

void    diagonale_display(                  FILE *fp,
                          diag_t diagonale,
                          elemento_t *vet_elementi);

boolean almeno_un_elemento(                 diag_t diagonale);

boolean almeno_un_elemento_acr(             diag_t diagonale,
                               elemento_t *vet_elementi);

boolean almeno_un_elemento_acr_avanti(      diag_t diagonale,
                                      elemento_t *vet_elementi);

boolean almeno_un_elemento_acr_indietro(    diag_t diagonale,
                                        elemento_t *vet_elementi);
boolean trova_el_acrobatici_in_sequenza(    diag_t diagonale,
                                        elemento_t *vet_elementi);

boolean verifica_condizioni_esistenza(      diag_t diagonale,
                                      elemento_t *vet_elementi);

boolean verifica_condizioni_accettabili(    diag_t *diagonale,
                                        int n_diagonali,
                                        elemento_t *vet_elementi);

int     calcola_diffclt(                    diag_t diagonale,
                        elemento_t *vet_elementi);

float calcola_valore(                       diag_t diagonale,
                     elemento_t *vet_elementi);

#endif
