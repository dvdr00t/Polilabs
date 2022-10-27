# SOME VARIABLES
# --------------
N := 5000					# Dimension of the arrays and the matrix


target:
	@ clear
	@ rm -f exe03
	@ gcc -Wall -o exe03 solution.c
	@ ./exe03 $(N)

targetA:
	@ clear
	@ rm -f exe03_A
	@ gcc -Wall -o exe03_A solutionA.c
	@ ./exe03_A $(N)

targetB:
	@ clear
	@ rm -f exe03_B
	@ gcc -Wall -o exe03_B solutionB.c
	@ ./exe03_B $(N)

clean:
	@ clear
	@ rm -f exe03
	@ rm -f exe03_A
	@ rm -f exe03_B
	@ echo "All shiny..."