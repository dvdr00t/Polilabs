# EXERCISE 06
## Code implementation with processes and signals

Write a C program that is able to handle signals as described by the following
specifications.
The program accepts signals `SIGUSR1` and `SIGUSR2`, and:
- It displays a "success" message every time it receives a signal `SIGUSR1` followed by a signal `SIGUSR2`, or vice-versa.
- It displays an error message every time it receives two signals `SIGUSR1` or two signals `SIGUSR2` consecutively.
- It terminates if it receives three successive `SIGUSR1` or `SIGUSR2` signals.


### Suggestion
Once it is compiled, run the program in the background (...&) and use the shell command "kill" to send signals SIGUSR1 and SIGUSR2 to the process.
