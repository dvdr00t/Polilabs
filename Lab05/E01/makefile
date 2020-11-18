target:

	# Target make to compile the .c program.
	# It generated the "test" executable
	@clear
	@gcc -Wall -o test script_L05E01.c
	@echo "File compiled correctly"

# A part of the exercise
running_A:
	@clear

	# <  changes the stdin to inFile.txt
	# 1> changes the stdout to evenFile.txt
	# 2> changes the stderr to oddFile.txt 
	@./test < inFile.txt 1> evenFile.txt 2> oddFile.txt

	# Printing results
	@echo ">>> evenFile.txt:"
	@cat evenFile.txt 
	@echo 
	@echo ">>> oddFile.txt:"
	@cat oddFile.txt 

# B part of the exercise
running_B:
	@clear

	# wc -l counts the lines of the input
	# wc -w counts the words of the input
	# ps -U [name] shows the processes owned by [name]
	# tr -d " " deletes all the spaces in the input

	@echo "Number of files/directoris in /home --> \c"
	@ls -l /home | wc -w | tr -d " "

	@echo "Number of strings in lab01e01in.txt--> \c"
	@cat lab01e01in.txt | wc -w | tr -d " "

	@echo "Number of lines in lab01e01in.txt--> \c"
	@cat lab01e01in.txt | wc -l | tr -d " "

	@echo "Number of processes owned by the root user --> \c"
	@ps -U root | wc -l | tr -d " "

	
