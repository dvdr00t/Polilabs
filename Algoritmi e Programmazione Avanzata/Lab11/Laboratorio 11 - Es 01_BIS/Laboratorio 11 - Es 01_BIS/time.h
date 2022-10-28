//
//  time.h
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//

#ifndef TIME_H
#define TIME_H

#include <stdio.h>

/* --- DATA --- */

typedef struct {
    int hours;
    int minutes;
} tm_t;

typedef struct {
    int year;
    int month;
    int day;
    tm_t time;
} date_t;


/* --- FUNCTIONS --- */

//Input and output
void date_READ (FILE *fp, date_t *date);
void time_READ (FILE *fp, tm_t *time);
void date_SHOW (date_t date);
void time_SHOW (tm_t time);

//Operations
int date_COMPARE (date_t date_A, date_t date_B);
void date_COPY (date_t *date_DST, date_t date_SRC);
void time_COPY (tm_t *time_DST, tm_t time_SRC);

#endif /* TIME_H */
