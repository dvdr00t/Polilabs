//
//  inv.c
//  Laboratorio 9 - Es 2
//
//  Created by Davide Arcolini on 17/12/2019.
//  Copyright © 2019 Davide Arcolini. All rights reserved.
//

#include "inv.h"

/* ---- FUNCTIONS ---- */

//INPUT AND OUTPUT for stats
void inv_statREAD (FILE *fp, stat_t *stat) {
    fscanf(fp, "%d %d %d %d %d %d", &stat->hp, &stat->mp, &stat->atk, &stat->def, &stat->mag, &stat->spr);
}
void inv_statSHOW (FILE *fp, stat_t *stat) {
    fprintf(fp, "%d %d %d %d %d %d", stat->hp, stat->mp, stat->atk, stat->def, stat->mag, stat->spr);
}

//INPUT AND OUTPUT for inventory
void inv_invREAD (FILE *fp, inv_t *inv) {
    fscanf(fp, "%s %s", inv->nameWeapon, inv->typeWeapon);
    inv_statREAD(fp, &inv->stats);
}
void inv_invSHOW (FILE *fp, inv_t *inv) {
    fprintf(fp, "%s %s ", inv->nameWeapon, inv->typeWeapon);
    inv_statSHOW(fp, &inv->stats);
}
