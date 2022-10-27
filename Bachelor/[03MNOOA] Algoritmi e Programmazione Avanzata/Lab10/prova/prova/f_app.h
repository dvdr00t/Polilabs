//
//  f_app.h
//  prova
//
//  Created by Davide Arcolini on 24/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef __F_APP__
#define __F_APP__
#include <stdio.h>
#include "elementi.h"
#include "f_ric.h"

typedef struct criterio{
    float   valore_specifico;
    boolean avanti;
    boolean indietro;
    boolean sequenza;
}criterio_t;


float valore_specifico(elemento_t elemento);
criterio_t funzione_appetibilita(elemento_t elemento1,elemento_t elemento2);
criterio_t funzione_appetibilita_entrata(elemento_t elemento);


void calcola_diagonale(int n_elementi,elemento_t *vet_elementi,diag_t *diagonali,int riga,int colonna);
int valuta_elemento(int n_elementi,elemento_t *vet_elementi,diag_t *diagonali,int riga,int colonna,int DD,int DP,int diag_diff,int tot_diff);





#endif
