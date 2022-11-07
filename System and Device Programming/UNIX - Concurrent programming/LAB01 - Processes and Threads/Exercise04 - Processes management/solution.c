#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

int generate_subprocesses(int n, int current_h, int final_h);
int main(int args, char *argv[]) {

	/* CHECKING INPUT FROM COMMAND LINE */
	if (args != 3/* || isdigit(atoi(argv[1])) == 0 || isdigit(atoi(argv[2])) == 0*/) {
		fprintf(stderr, "[USAGE] ./prg <height_h> <degree_n>\n\n");
		exit(EXIT_FAILURE);
	}

	/* PARSING INPUT FROM COMMAND LINE */
	int h = atoi(argv[1]);
	int n = atoi(argv[2]);

	/* GENERATING THE PROCESS TREE */
	pid_t pid;
	fprintf(stdout, "[PARENT - %d] I am the root.\n", getpid());
	for (int i = 0; i < n; i++) {
		pid = fork();
		if (pid == -1) {
			fprintf(stderr, "[ERROR] fork() failed execution.\n\n");
			exit(EXIT_FAILURE);
		} else if (pid > 0) {
			/* PARENT */
		} else {
			/* CHILDREN */
			if (h == 1) {
				fprintf(stdout, "[LEAF - %d] I am a leaf!\n", getpid());
				break;
			} else {
				if (generate_subprocesses(n, 2, h) != 0) {
					fprintf(stderr, "[ERROR] process %d failed execution of generate_subprocesses().\n\n", pid);
					exit(EXIT_FAILURE);
				}
				break;
			}
			
		}
	}
	/* WATING FOR THE CHILD PROCESSES */
	int status;
	if (pid > 0) {
		/* PARENT */
		while (n > 0) {
			pid = wait(&status);
			fprintf(stdout, "[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
			n--;
		}
	}
	exit(EXIT_SUCCESS);
}

/**
 * @brief Generates recursively a subtree of the given height with the given degree.
 * 
 * @param n 
 * @param current_h 
 * @param final_h 
 * @return int 
 */
int generate_subprocesses(int n, int current_h, int final_h) {
	pid_t pid;
	for (int i = 0; i < n; i++) {
		pid = fork();
		if (pid == -1) {
			fprintf(stderr, "[ERROR] fork() failed execution.\n\n");
			exit(EXIT_FAILURE);
		} else if (pid > 0) {
			/* PARENT */
		} else {
			if (current_h == final_h) {
				fprintf(stdout, "[LEAF - %d] I am a leaf! My father is: %d\n", getpid(), getppid());
				return 0;
			} else {
				current_h++;
				if (generate_subprocesses(n, current_h, final_h) != 0) {
					fprintf(stderr, "[ERROR] process %d failed execution of generate_subprocesses().\n\n", pid);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	/* WATING FOR THE CHILD PROCESSES */
	int status;
	if (pid > 0) {
		/* PARENT */
		while (n > 0) {
			pid = wait(&status);
			fprintf(stdout, "[PARENT] Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
			n--;
		}
	}
	return 0;
}