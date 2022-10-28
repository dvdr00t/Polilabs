//
//  time.c
//  Laboratorio 11 - Es 01_BIS
//
//  Created by Davide Arcolini on 16/01/2020.
//  Copyright © 2020 Davide Arcolini. All rights reserved.
//


#include "time.h"

/* --- FUNCTIONS --- */

//Input and output
void date_READ (FILE *fp, date_t *date) {
    
    if (fp == stdin) {
        fscanf(fp, "%d/%d/%d", &date->year, &date->month, &date->day);
        return;
    }
    
    fscanf(fp, "%d/%d/%d", &date->year, &date->month, &date->day);
    time_READ(fp, &date->time);
}
void time_READ (FILE *fp, tm_t *time) {
    fscanf(fp, "%d:%d", &time->hours, &time->minutes);
}
void date_SHOW (date_t date) {
    fprintf(stdout, "\t\t%04d/%02d/%02d ", date.year, date.month, date.day);
    //time_SHOW(date.time);
}
void time_SHOW (tm_t time) {
    fprintf(stdout, "%02d:%02d ", time.hours, time.minutes);
}

//Operations
int date_COMPARE (date_t date_A, date_t date_B) {
    if (date_A.year < date_B.year)
        return -1;
    else if (date_A.year > date_B.year)
        return 1;
    else {
        if (date_A.month < date_B.month)
            return -1;
        else if (date_A.month > date_B.month)
            return 1;
        else {
            if (date_A.day < date_B.day)
                return -1;
            else if (date_A.day > date_B.day)
                return 1;
            else
                return 0;
        }
    }
}
void date_COPY (date_t *date_DST, date_t date_SRC) {
    date_DST->year = date_SRC.year;
    date_DST->month = date_SRC.month;
    date_DST->day = date_SRC.day;
    time_COPY(&date_DST->time, date_SRC.time);
}
void time_COPY (tm_t *time_DST, tm_t time_SRC) {
    time_DST->hours = time_SRC.hours;
    time_DST->minutes = time_SRC.minutes;
}
