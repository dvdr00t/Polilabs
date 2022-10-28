#include <stdio.h>
#include <stdlib.h>

void binary(int *, int, int);

int
main (
  int argc,
  char *argv[]
  )
{
  int n;
  int *vet;

  n = atoi (argv[1]);

  vet = (int *)malloc(n * sizeof(int));
  if (vet == NULL) {
    printf("Allocatin Error.\n");
    exit(1);
  }

  printf("Binary Numbers:\n");
  binary(vet, n, 0);
  free(vet);

  return 0;
}

void
binary (
  int *vet,
  int n,
  int i
  )
{
  int j;

  if (i >= n) {
    for (j=0; j<n; j++) {
      printf("%d", vet[j]);
    }
    printf("\n");
    return;
  }

  vet[i] = 0;
  binary(vet, n, i+1);

  vet[i] = 1;
  binary(vet, n, i+1);

  return;
}



