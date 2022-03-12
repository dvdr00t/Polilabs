#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

typedef struct matrix {
	int **A;
	int **B;
	int **C;
	int rows_A;
	int rows_B;
	int columns_A;
	int columns_B;
	int index;
} matrix_t;

void* routine(void* data);
void print_matrix(int **mat, int rows, int columns);
void mat_mul (int **A, int **B, int r, int x, int c, int **C);
matrix_t matrix_INIT(int **A, int **B, int **C, int rows_A, int rows_B, int columns_A, int columns_B);

/* MAIN FUNCTION */
int main(int args, char *argv[]) {

	/* CHECKING INPUT FROM COMMAND LINE */
	if (args != 2) {
		fprintf(stderr, "[USAGE] ./prg <filename>\n\n");
		exit(EXIT_FAILURE);
	}

	/* READING INPUT FILE */
	char* filename = argv[1];
	FILE *fp_input = fopen(filename, "r");
	if (fp_input == NULL) {
		fprintf(stderr, "[ERROR] opening %s failed.\n\n", filename);
		exit(EXIT_FAILURE);
	}
	int rows_A, columns_A, rows_B, columns_B;
	fscanf(fp_input, "%d %d %d %d\n", &rows_A, &columns_A, &rows_B, &columns_B);

	/* ALLOCATING SPACE FOR INPUT MATRICES */
	int **A = (int**) malloc(rows_A * sizeof(int *));
	if (A == NULL) {
		fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < rows_A; i++) {
		A[i] = (int*) malloc(columns_A * sizeof(int));
		if (A[i] == NULL) {
			fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
			exit(EXIT_FAILURE);
		}
	}

	int **B = (int**) malloc(rows_B * sizeof(int *));
	if (B == NULL) {
		fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < rows_B; i++) {
		B[i] = (int*) malloc(columns_B * sizeof(int));
		if (B[i] == NULL) {
			fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
			exit(EXIT_FAILURE);
		}
	} 

	/* READING MATRICES FROM FILE */
	for (int i = 0; i < rows_A; i++) {
		for (int j = 0; j < columns_A; j++) {
			fscanf(fp_input, "%d", &A[i][j]);
		}
	}
	print_matrix(A, rows_A, columns_A);
	for (int i = 0; i < rows_B; i++) {
		for (int j = 0; j < columns_B; j++) {
			fscanf(fp_input, "%d", &B[i][j]);
		}
	}
	print_matrix(B, rows_B, columns_B);


	/* COMPUTING RESULT MATRIX IN MULTITHREAD */
	int **C;
	mat_mul(A, B, rows_A, columns_A, columns_B, C);



	/* TERMINATING EXECUTION */
	free(A);
	free(B);
	fclose(fp_input);
	exit(EXIT_SUCCESS);
}

/**
 * @brief Print a formatted matrix.
 * 
 * @param mat 
 * @param rows 
 * @param columns 
 */
void print_matrix(int **mat, int rows, int columns) {
	fprintf(stdout, "\n[MATRIX]\n");
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			fprintf(stdout, "%d ", mat[i][j]);
		}
		fprintf(stdout, "\n");
	}
	fprintf(stdout, "\n");
	return;
}

/**
 * @brief Main function to compute the result matrix.
 * 
 * @param A 
 * @param B 
 * @param r 
 * @param x 
 * @param c 
 * @param C 
 */
void mat_mul (int **A, int **B, int r, int x, int c, int **C) {

	/* ALLOCATING SPACE FOR RESULT MATRIX */
	C = (int**) malloc(r * sizeof(int*));
	if (C == NULL) {
		fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < r; i++) {
		C[i] = (int*) malloc(c * sizeof(int));
		if (C[i] == NULL) {
			fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
			exit(EXIT_FAILURE);
		}
	}

	/* ALLOCATING SPACE FOR THREADS_ID */
	pthread_t* threads_ID = (pthread_t*) malloc((r*c) * sizeof(pthread_t));
	if (threads_ID == NULL) {
		fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
		exit(EXIT_FAILURE);
	}

	/* GENERATING THREADS */
	matrix_t matrix_info = matrix_INIT(A, B, C, r, x, x, c);
	for (int index = 0; index < r*c; index++) {
		matrix_info.index = index;
		if (pthread_create(&threads_ID[index], NULL, routine, (void*) &matrix_info) != 0) {				
			fprintf(stderr, "[ERROR] pthread_create() failed execution.\n\n");
			exit(EXIT_FAILURE);
		}
	}

	/* JOINING THREADS */
	int exit_status;
	for (int i = 0; i < r*c; i++) {
		if (pthread_join(threads_ID[i], &exit_status) != 0) {
			fprintf(stderr, "[ERROR] pthread_create() failed execution.\n\n");
			exit(EXIT_FAILURE);
		}
	}

	/* PRINTING RESULT ON FILE */
	FILE *fp_output = fopen("file_output.txt", "w");
	if (fp_output == NULL) {
		fprintf(stderr, "[ERROR] fopen() failed execution.\n\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			fprintf(fp_output, "%d ", matrix_info.C[i][j]);
		}
		fprintf(fp_output, "\n");
	}
	fprintf(fp_output, "\n");


	/* TERMINATING EXECUTION */
	free(threads_ID);
	free(C);
	fclose(fp_output);
	return;
}

/**
 * @brief Initializa the matrices information data type to be passed to the thread's routine.
 * 
 * @param A 
 * @param B 
 * @param C 
 * @param rows_A 
 * @param rows_B 
 * @param columns_A 
 * @param columns_B 
 * @return matrix_t 
 */
matrix_t matrix_INIT(int **A, int **B, int **C, int rows_A, int rows_B, int columns_A, int columns_B) {
	matrix_t matrix;
	
	/* INITIALIZATION */
	matrix.A = A;
	matrix.B = B;
	matrix.C = C;
	matrix.rows_A = rows_A;
	matrix.rows_B = rows_B;
	matrix.columns_A = columns_A;
	matrix.columns_B = columns_B;
	matrix.index = 0;

	return matrix;
}

/**
 * @brief Routine executed by the thread.
 * 
 * @param data 
 * @return void* 
 */
void* routine(void* data) {

	/* CONVERTING TYPE */
	matrix_t* matrix_info = (matrix_t*) data;

	/* COMPUTING VALUE */
	int index_R = matrix_info->index / matrix_info->rows_A;
	int index_C = matrix_info->index % matrix_info->columns_B;
	for (int i = 0; i < matrix_info->columns_A; i++) {
		matrix_info->C[index_R][index_C] += matrix_info->A[index_R][i] * matrix_info->B[i][index_C];
		print_matrix(matrix_info->C, matrix_info->rows_A, matrix_info->columns_B);
	}

	pthread_exit(NULL);
}