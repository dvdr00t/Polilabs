//
//  elementi.c
//  prova
//
//  Created by Davide Arcolini on 24/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#include "elementi.h"



void        elemento_display(FILE* fp,elemento_t elemento){
    fprintf(fp,"\n%s",elemento.nome);
}

boolean     elementi_compatibili(elemento_t elemento1,elemento_t elemento2){
    if (elemento1.tipo == TRANS || elemento2.tipo == TRANS )
        return FALSE;
    if (elemento1.direzione_usc == elemento2.direzione_ingr)
        return TRUE;
    return FALSE;
}

boolean     entrata_frontale(elemento_t elemento){
    if (elemento.direzione_ingr == FRONTALE)
        return TRUE;
    return FALSE;
}

int         estrai_difficolta(elemento_t elemento){
    return elemento.diffic;
}

float       estrai_valore(elemento_t elemento){
    return elemento.valore;
}

boolean     req_precedenza(elemento_t elemento){
    return elemento.precedenza;
}

boolean     finale(elemento_t elemento){
    return elemento.successivo;
}

tipo_acr    estrai_tipo_acr(elemento_t elemento){
    return elemento.tipo;
}
