#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* STORING HISTORY INFORMATION ABOUT SIGNALS */
#define history_size 3
int signal_history[history_size];

void manager (int sig) {
	
	/* CHECKING SIGNAL FROM THE USER */
	if (sig != SIGUSR1 && sig != SIGUSR2) {
		fprintf(stderr, "[ERROR] %d is a non-valid signal.\n", sig);
	} 

	/* EVALUATING HISTORY */
	if (signal_history[0] == -1) {
		fprintf(stdout, "[SIGNAL] received signal %d...\n", sig);
		signal_history[0] = sig;
	} else {
		if (signal_history[1] == -1) {
			if (signal_history[0] == sig) {
				fprintf(stdout, "[WARNING] received the same signal twice in a row. Ready to quit execution.\n");
				signal_history[1] = sig;
			} else {
				fprintf(stdout, "[SUCCESS] nice! You rock that signals.\n");
				signal_history[0] = sig;
			}
		} else {
			if (signal_history[1] == sig) {
				fprintf(stdout, "[ENDING] received the same signal three times in a row. Quitting execution...\n\n");
				exit(EXIT_SUCCESS);
			} else {
				fprintf(stdout, "[SUCCESS] nice! You rock that signals.\n");
				signal_history[1] = -1;
				signal_history[0] = sig;
			}
		}
	}
}
int main(int args, char *argv[]) {

	/* CHECKING INPUT FROM COMMAND LINE */
	if (args != 1) {
		fprintf(stderr, "[USAGE] ./prg\n\n");
		exit(EXIT_FAILURE);
	}

	/* PRINTING INFORMATION TO THE USER */
	fprintf(stdout, "\n\n-------------------------------------\n");
	fprintf(stdout, "... this process has PID = %d ...\n", getpid());
	fprintf(stdout, "... send kill signals to this PID...\n");
	fprintf(stdout, "-------------------------------------\n");

	/* HANDLING SIGNALS */
	for (int i = 0; i < history_size; i++) {
		signal_history[i] = -1;
	}
	signal(SIGUSR1, manager);
	signal(SIGUSR2, manager);

	/* WAITING FOR THE USER SIGNALS */
	while (1);
}
