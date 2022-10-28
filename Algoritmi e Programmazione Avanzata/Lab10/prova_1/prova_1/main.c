//
//  main.c
//  prova_1
//
//  Created by Davide Arcolini on 24/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "elementi.txt"
#define NDIAG 3
#define MAXE 5
#define MAXL 100
#define dd 11
#define dp 33

typedef struct elemento_s {
    char *nome;
    int tipo;
    int in, out;
    int finale;
    int prec;
    int diff;
    float val;
} elemento_t;

typedef struct diagonale_s {
    int *elementi;
    int ne;
    int diff;
    float val;
} diagonale_t;

typedef struct diagonali_s {
    diagonale_t *vd;
    int nd, maxd;
} diagonali_t;

typedef struct elementi_s {
    elemento_t *ve;
    int ne;
} elementi_t;

elementi_t *leggiElementi() {
    int i;
    char nome[MAXL];
    FILE *in = fopen(FILENAME, "r");
    elementi_t *E = malloc(1 * sizeof(*E));
    if (E == NULL) return NULL;
    fscanf(in, "%d", &E->ne);
    E->ve = calloc(E->ne, sizeof(elemento_t));
    if (E->ve == NULL) return NULL;
    for(i=0;i<E->ne;i++) {
        fscanf(in, "%s %d %d %d %d %d %f %d\n", nome, &E->ve[i].tipo, &E->ve[i].in, &E->ve[i].out, &E->ve[i].prec, &E->ve[i].finale, &E->ve[i].val, &E->ve[i].diff);
        E->ve[i].nome = strdup(nome);
    }
    return E;
}

void disp_rip(elementi_t *E, diagonali_t *D, int DD, int pos, int diff, int acro, int direzione, int finale, int *diag) {
    int i;
    if (pos > 0) {
        if (acro > 0) {
            if (D->nd == D->maxd) {
                D->maxd *= 2;
                D->vd = realloc(D->vd, D->maxd * sizeof(diagonale_t));
                if (D->vd == NULL) {
                    /* Situazione di errore non recuperabile... */
                    exit(-1);
                }
            }
            D->vd[D->nd].diff = diff;
            D->vd[D->nd].ne = pos;
            D->vd[D->nd].elementi = malloc(pos *sizeof(int));
            D->vd[D->nd].val = 0.0;
            for(i=0;i<pos;i++) {
                D->vd[D->nd].val += E->ve[diag[i]].val;
                D->vd[D->nd].elementi[i] = diag[i];
            }
            D->nd++;
        }
    }
    
    if (pos >= MAXE || finale)
        return;
    
    for(i=0;i<E->ne;i++) {
        if (diff+E->ve[i].diff > DD) continue;
        if (direzione != E->ve[i].in) continue;
        if (pos == 0 && E->ve[i].prec) continue;
        diag[pos] = i;
        disp_rip(E, D, DD, pos+1, diff+E->ve[i].diff, acro+E->ve[i].tipo, E->ve[i].out, E->ve[i].finale, diag);
    }
}

void generaDiagonali(elementi_t *E, diagonali_t *D, int DD) {
    int *diag = calloc(MAXE, sizeof(int));
    disp_rip(E, D, DD, 0, 0, 0, 1, 0, diag);
}

diagonali_t *initDiagonali() {
    diagonali_t *D = malloc(1 * sizeof(*D));
    D->vd = calloc(1, sizeof(diagonale_t));
    D->nd = 0;
    D->maxd = 1;
    return D;
}

int checkValido(elementi_t *E, diagonali_t *D, int *prog, int nd, float *val, int *B) {
    int i, j, bonus = -1;
    int fwd = 0, bwd = 0, seq = 0;
    for(i=0;i<nd;i++) {
        int dseq = 0;
        for(j=0;j<D->vd[prog[i]].ne;j++) {
            if (E->ve[D->vd[prog[i]].elementi[j]].tipo == 2) {
                fwd = 1;
                dseq++;
            } else if (E->ve[D->vd[prog[i]].elementi[j]].tipo == 1) {
                bwd = 1;
                dseq++;
            } else {
                dseq = 0;
            }
            if (j==(D->vd[prog[i]].ne-1) && E->ve[D->vd[prog[i]].elementi[j]].tipo > 0 && E->ve[D->vd[prog[i]].elementi[j]].diff > 7) {
                if (bonus == -1 || (D->vd[prog[bonus]].val < D->vd[prog[i]].val)) bonus = i;
            }
            if (dseq >= 2) seq = 1;
        }
    }
    if (fwd && bwd && seq) {
        for(i=0;i<nd;i++) {
            (*val) += D->vd[prog[i]].val * ((i==bonus) ? 1.50 : 1.00);
        }
        *B = bonus;
        return 1;
    }
    return 0;
}

void comb_rip(elementi_t *E, diagonali_t *D, int DP, int pos, int *prog, int diff, int *bestprog, float *bestval, int *bestbonus, int *numd, int start) {
    int i, bonus;
    float val = 0.0;
    if (pos >= NDIAG) {
        if (checkValido(E, D, prog, pos, &val, &bonus)) {
            if (val > *bestval) {
                *numd = pos;
                *bestval = val;
                *bestbonus = bonus;
                memcpy(bestprog, prog, pos*sizeof(int));
            }
        }
        return;
    }
    
    for(i=start;i<D->nd;i++) {
        if (diff+D->vd[i].diff > DP) continue;
        prog[pos] = i;
        comb_rip(E, D, DP, pos+1, prog, diff+D->vd[i].diff, bestprog, bestval, bestbonus, numd, i);
    }
}

void generaProgramma(elementi_t *E, diagonali_t *D, int DP) {
    int i, j, bonus, d, numd = 0, *prog = malloc(NDIAG * sizeof(int)), *bestprog = malloc(NDIAG * sizeof(int));
    float bestval = -1.0;
    comb_rip(E, D, DP, 0, prog, 0, bestprog, &bestval, &bonus, &numd, 0);
    if (numd > 0) {
        printf("TOT = %f\n", bestval);
        for(i=0;i<numd;i++) {
            d = bestprog[i];
            printf("DIAG %d > %.3f %s\n", d, D->vd[d].val, ((bonus == i) ? "* 1.5 (BONUS)" : ""));
            for(j=0;j<D->vd[d].ne;j++) printf("%s ", E->ve[D->vd[d].elementi[j]].nome);
            printf("\n");
        }
    }
}

int main(int argc, char **argv) {
    int DD = dd, DP = dp;
    diagonali_t *D = initDiagonali();
    elementi_t *E = leggiElementi();
    printf("Generazione diagonali\n");
    generaDiagonali(E, D, DD);
    printf("Sono disponibili %d diagonali\n", D->nd);
    printf("Generazione programmi\n");
    generaProgramma(E, D, DP);
    return 0;
}
