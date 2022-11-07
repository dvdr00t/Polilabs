#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/wait.h>

int generate_binary_number(int value, int size);
int main(int args, char *argv[]) {

	/* CHECKING INPUT FROM COMMAND LINE */
	if (args != 2/* || isdigit(atoi(argv[1])) == 0*/) {
		fprintf(stderr, "[USAGE] ./prg <size_of_binary_number>\n\n");
		exit(EXIT_FAILURE);
	}

	/* PARSING INPUT FROM COMMAND LINE */
	int n = atoi(argv[1]);

	/* GENERATING THE PROCESS TREE */
	pid_t pid;
	fprintf(stdout, "[PARENT - %d] I am the root.\n", getpid());
	for (int i = 0; i < pow(2, n); i++) {
		pid = fork();
		if (pid == -1) {
			fprintf(stderr, "[ERROR] fork() failed execution.\n\n");
			exit(EXIT_FAILURE);
		} else if (pid > 0) {
			/* PARENT */
		} else {
			/* CHILD */
			if (generate_binary_number(i, n) != 0) {
				fprintf(stderr, "[ERROR] child process failed to execute generate_binary_number().\n\n");
				exit(EXIT_FAILURE);
			}
			break;
		}
	}
	/* WATING FOR THE CHILDREN PROCESSES */
	int status;
	if (pid > 0) {
		/* PARENT */
		int children = pow(2, n);
		while (children > 0) {
			pid = wait(&status);
			if (status == 0) {
				fprintf(stdout, "[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
				children--;
			} else {
				fprintf(stderr, "[ERROR] stopping execution, child with PID %d failed.\n\n", pid);
				exit(EXIT_FAILURE);
			}
		}
	}
	exit(EXIT_SUCCESS);
}

/**
 * @brief Given the number of bits (size) and a number to be represented (value), it generates the correspondent binary number and prints it to the standard output.
 * 
 * @param value 
 * @param size 
 * @return int 
 */
int generate_binary_number(int value, int size) {

	/* ALLOCATING SPACE IN HEAP */
	int *binary_number = (int*) malloc(size*sizeof(int));
	if (binary_number == NULL) {
		fprintf(stderr, "[ERROR] malloc() failed execution.\n\n");
		return -1;
	}

	/* GENERATING BINARY NUMBER */
	for(int i = 0; i < size; i++) {    
		binary_number[i] = value % 2;    
		value = value / 2;    
	}    

	/* PRIINTING BINARY NUMBER */
	fprintf(stdout, "[CHILD - %d] Generating: ", getpid());
	for (int i = 0; i < size; i++) {
		fprintf(stdout, "%d", binary_number[i]);
	}
	fprintf(stdout, "\n");

	/* CLEANING SPACE */
	free(binary_number);
	return 0;

}