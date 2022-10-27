//
//  f_ric.c
//  prova
//
//  Created by Davide Arcolini on 24/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "f_ric.h"

diag_t diagonale_init( void) {
    diag_t tmp;
    tmp.cnt_elementi    = 0;
    tmp.valore          = 0;
    return tmp;
}

void diagonale_display(FILE *fp,diag_t diagonale,elemento_t *vet_elementi){
    int i;
    for ( i = 0; i < diagonale.cnt_elementi; i++){
        elemento_display(fp,vet_elementi[diagonale.diagonale[i]]);
    }
}

boolean almeno_un_elemento(diag_t diagonale){
    if (diagonale.cnt_elementi > 0)
        return TRUE;
    return FALSE;
}

boolean almeno_un_elemento_acr(diag_t diagonale,elemento_t *vet_elementi){
    int i;
    for ( i = 0; i < diagonale.cnt_elementi; i++){
        if (vet_elementi[diagonale.diagonale[i]].tipo == ACR_AVA || vet_elementi[diagonale.diagonale[i]].tipo == ACR_IND)
            return TRUE;
    }
    return FALSE;
}

boolean almeno_un_elemento_acr_avanti(diag_t diagonale,elemento_t *vet_elementi){
    int i;
    for ( i = 0; i < diagonale.cnt_elementi; i++){
        if (vet_elementi[diagonale.diagonale[i]].tipo == ACR_AVA)
            return TRUE;
    }
    return FALSE;
}


boolean almeno_un_elemento_acr_indietro(diag_t diagonale,elemento_t *vet_elementi){
    int i;
    for ( i = 0; i < diagonale.cnt_elementi; i++){
        if (vet_elementi[diagonale.diagonale[i]].tipo == ACR_IND)
            return TRUE;
    }
    return FALSE;
}

boolean trova_el_acrobatici_in_sequenza(diag_t diagonale,elemento_t *vet_elementi){
    int i;
    for ( i = 1; i < diagonale.cnt_elementi; i++){
        if (elementi_compatibili(vet_elementi[diagonale.diagonale[i-1]],vet_elementi[diagonale.diagonale[i]]))
            return TRUE;
    }
    return FALSE;
}

boolean verifica_condizioni_esistenza(diag_t diagonale,elemento_t *vet_elementi) {
    if (almeno_un_elemento(diagonale) && almeno_un_elemento_acr(diagonale,vet_elementi))
        return TRUE;
    return FALSE;
}

boolean verifica_condizioni_accettabili(diag_t *diagonale,int n_diagonali,elemento_t *vet_elementi){
    boolean almeno_un_elemento_acrobatico_avanti    = FALSE;
    boolean almeno_un_elemento_acrobatico_indietro  = FALSE;
    boolean almeno_due_elementi_acrobatici_in_seq   = FALSE;
    int i = 0;
    
    for ( i = 0; i < n_diagonali; i++){
        if (verifica_condizioni_esistenza(diagonale[i],vet_elementi) == FALSE)
            return FALSE;
    }
    
    for (int i = 0; i < n_diagonali; i++){
        if (almeno_un_elemento_acr_avanti(diagonale[i],vet_elementi))
            almeno_un_elemento_acrobatico_avanti    = TRUE;
        if (almeno_un_elemento_acr_indietro(diagonale[i],vet_elementi))
            almeno_un_elemento_acrobatico_indietro  = TRUE;
        if (trova_el_acrobatici_in_sequenza(diagonale[i],vet_elementi))
            almeno_due_elementi_acrobatici_in_seq   = TRUE;
    }
    return (almeno_un_elemento_acrobatico_avanti &&
            almeno_un_elemento_acrobatico_indietro &&
            almeno_due_elementi_acrobatici_in_seq);
}

int calcola_diffclt(diag_t diagonale,elemento_t *vet_elementi){
    int i,somma = 0;
    
    for ( i = 0; i < diagonale.cnt_elementi; i++){
        somma += vet_elementi[diagonale.diagonale[i]].diffic;
    }
    return somma;
}

float calcola_valore(diag_t diagonale,elemento_t *vet_elementi){
    int i = 0;
    float valore = 0;
    for ( i = 0; i < diagonale.cnt_elementi; i++){
        valore += vet_elementi[diagonale.diagonale[i]].valore;
    }
    return valore;
}
