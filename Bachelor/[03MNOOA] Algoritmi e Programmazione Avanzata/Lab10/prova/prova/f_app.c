//
//  f_app.c
//  prova
//
//  Created by Davide Arcolini on 24/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "f_app.h"

#define N_DIAGONALI 3
#define MAX_ELEMENTI 5

static boolean el_avanti    = FALSE;
static boolean el_indietro  = FALSE;
static boolean sequenza     = FALSE;

float valore_specifico(elemento_t elemento) {
    return (elemento.valore/elemento.diffic);
}

criterio_t funzione_appetibilita(elemento_t elemento1,elemento_t elemento2){
    criterio_t tmp;
    tmp.avanti = FALSE;
    tmp.indietro = FALSE;
    tmp.sequenza = FALSE;
    
    if (elemento2.tipo == ACR_AVA)
        tmp.avanti = TRUE;
    if (elemento2.tipo == ACR_IND)
        tmp.indietro = TRUE;
    if (elemento1.direzione_usc == elemento2.direzione_ingr)
        tmp.sequenza = TRUE;
    
    tmp.valore_specifico = valore_specifico(elemento2);
    
    return tmp;
}

criterio_t funzione_appetibilita_entrata(elemento_t elemento){
    criterio_t tmp;
    tmp.avanti = FALSE;
    tmp.indietro = FALSE;
    tmp.sequenza = FALSE;
    
    if (elemento.tipo == ACR_AVA)
        tmp.avanti = TRUE;
    if (elemento.tipo == ACR_IND)
        tmp.indietro = TRUE;
    
    tmp.valore_specifico = valore_specifico(elemento);
    
    return tmp;
}

int valuta_elemento(int n_elementi,elemento_t *vet_elementi,diag_t *diagonali,int riga,int colonna,int DD,int DP,int diag_diff,int tot_diff){
    criterio_t best_elemento;
    best_elemento.valore_specifico  = -1.0;
    best_elemento.avanti            = FALSE;
    best_elemento.indietro          = FALSE;
    best_elemento.sequenza          = FALSE;
    int indice                      = -1;
    float moltiplicatore;
    
    for (int i = 0; i < n_elementi; i++){
        if (colonna == 0){
            if (entrata_frontale(vet_elementi[i]) && diag_diff + vet_elementi[i].diffic <= DD && tot_diff + vet_elementi[i].diffic <= DP)
                if (funzione_appetibilita_entrata(vet_elementi[i]).valore_specifico > best_elemento.valore_specifico){
                    best_elemento   = funzione_appetibilita_entrata(vet_elementi[i]);
                    indice          = i;
                }
        }
        else {
            if (diag_diff + vet_elementi[i].diffic <= DD && tot_diff + vet_elementi[i].diffic <= DP){
                if (riga == 2 )
                    if (vet_elementi[i].diffic >= DIFF_BONUS)
                        moltiplicatore = BONUS;
                moltiplicatore = 1.0;
                if (funzione_appetibilita_entrata(vet_elementi[i]).valore_specifico*moltiplicatore > best_elemento.valore_specifico){
                    if (!el_avanti){
                        if (funzione_appetibilita_entrata(vet_elementi[i]).avanti){
                            best_elemento   = funzione_appetibilita_entrata(vet_elementi[i]);
                            indice          = i;
                        }
                    }
                    else if (!el_indietro){
                        if (funzione_appetibilita_entrata(vet_elementi[i]).indietro){
                            best_elemento   = funzione_appetibilita_entrata(vet_elementi[i]);
                            indice          = i;
                        }
                    }
                    else if (!sequenza){
                        if (funzione_appetibilita(vet_elementi[diagonali[riga].diagonale[colonna-1]],vet_elementi[i]).sequenza){
                            best_elemento   = funzione_appetibilita(vet_elementi[diagonali[riga].diagonale[colonna-1]],vet_elementi[i]);
                            indice          = i;
                        }
                    }
                    else {
                        if (funzione_appetibilita_entrata(vet_elementi[i]).valore_specifico > best_elemento.valore_specifico){
                            best_elemento   = funzione_appetibilita_entrata(vet_elementi[i]);
                            indice          = i;
                        }
                    }
                }
            }
        }
    }
    
    
    if (best_elemento.avanti)           //aggiorno le variabili globali così da dare la precedenza
        el_avanti   = TRUE;
    else if (best_elemento.indietro)
        el_indietro = TRUE;
    if (best_elemento.sequenza)
        sequenza    = TRUE;
    
    if (indice != -1){
        diagonali[riga].diagonale[colonna] = indice;
        diagonali[riga].cnt_elementi++;
        return vet_elementi[indice].diffic;
    }
    return -1;
}
