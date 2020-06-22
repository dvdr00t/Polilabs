
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#include "f_app.h"
#include "f_ric.h"
#include "elementi.h"

#define NOMEFILE "elementi.txt"
#define N_DIAGONALI 3
#define MAX_ELEMENTI 5


int main(int argc, char *argv[]) {
    
    char string[10][10];
    
    printf("%d\n", strlen(string));
    
    FILE* fp = fopen(NOMEFILE,"r");
    
    int n_elementi,DD,DP;
    
    elemento_t* vet_elementi;
    
    float best_val = 0.0;
    
    diag_t diagonali[N_DIAGONALI];
    
    for (int i = 0; i < N_DIAGONALI; i++){
        diagonali[i] = diagonale_init();
    }
    
    
    printf("Inserisci la DD e la DP: ");
    scanf("%d%d",&DD,&DP);
    
    fscanf(fp,"%d",&n_elementi);
    vet_elementi = (elemento_t*) malloc(n_elementi * sizeof(elemento_t));
    
    
    for (int i = 0; i < n_elementi; i++){
        fscanf(fp,"%s %d %d%d %d%d %f%d",vet_elementi[i].nome,&vet_elementi[i].tipo,&vet_elementi[i].direzione_ingr,&vet_elementi[i].direzione_usc,&vet_elementi[i].precedenza,&vet_elementi[i].successivo,&vet_elementi[i].valore,&vet_elementi[i].diffic);
    }
    
    int diag_diff     = 0;
    int tot_diff    = 0;
    int curr_diff;
    int min_diff_index;
    int min_diff = INT_MAX;
    
    for (int i = 0; i < n_elementi; i++){
        if (entrata_frontale(vet_elementi[i])){
            if (vet_elementi[i].diffic < min_diff){
                min_diff = vet_elementi[i].diffic;
                min_diff_index = i;
            }
        }
    }
    tot_diff = vet_elementi[min_diff_index].diffic*3;
    
    for (int i = 0; i < N_DIAGONALI; i++){
        diagonali[i].diagonale[0] = min_diff_index;
        diagonali[i].cnt_elementi++;
    }
    
    
    for (int i = N_DIAGONALI-1; i >= 0; i--){
        diag_diff = calcola_diffclt(diagonali[i],vet_elementi);
        for (int j = 1; j < MAX_ELEMENTI; j++){
            if ((curr_diff = valuta_elemento(n_elementi,vet_elementi,diagonali,i,j,DD,DP,diag_diff,tot_diff)) != -1){
                diag_diff +=     curr_diff;
                tot_diff  +=    curr_diff;
            }
            else break;    //scorro alla prosisma riga
        }
    }
    
    for (int i = 0; i < N_DIAGONALI; i++){
        printf("\nDIAGONALE %d",i+1);
        diagonale_display(stdout,diagonali[i],vet_elementi);
        printf("\nVALORE: %2f",calcola_valore(diagonali[i],vet_elementi));
    }
    
    free(vet_elementi);
    printf("\n");
    
    return(0);
}
