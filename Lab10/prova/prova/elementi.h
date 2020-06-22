//
//  elementi.h
//  prova
//
//  Created by Davide Arcolini on 24/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef __ELEMENTI__H__
#define __ELEMENTI__H__

#include <stdio.h>
#include <string.h>

#define BONUS 1.5
#define DIFF_BONUS 8
#define MAX_ELEMNTI 5

typedef enum{FALSE,TRUE} boolean;

typedef enum{TRANS,ACR_IND,ACR_AVA}tipo_acr;

typedef enum{SPALLE,FRONTALE} dir_t;



typedef struct elemento{
    char nome[100];
    
    tipo_acr tipo;
    
    dir_t direzione_ingr;
    dir_t direzione_usc;
    
    boolean precedenza;
    boolean successivo;
    
    float valore;
    
    int diffic;
}elemento_t;

elemento_t  elemento_init(  char*   nome,
                          tipo_acr tipo,
                          dir_t   direzione_ingr,
                          dir_t   direzione_usc,
                          boolean precedenza,
                          boolean successivo,
                          float   valore,
                          int     diffic);

void        elemento_display(   FILE*       fp,
                             elemento_t  elemento);

boolean     elementi_compatibili(   elemento_t elemento1,
                                 elemento_t elemento2);

boolean     entrata_frontale(       elemento_t elemento);

int         estrai_difficolta(      elemento_t elemento);

float       estrai_valore(          elemento_t elemento);

boolean     req_precedenza(         elemento_t elemento);

boolean     finale(                 elemento_t elemento);

tipo_acr    estrai_tipo_acr(        elemento_t elemento);
#endif
