#   VARIABLES
# -------------
N := 3

target:
	@clear
	@rm -f myLibrary.o	
	@rm -f solution.o
	@rm -f exe01
	@gcc -c myLibrary.c
	@gcc -o exe01 solution.c myLibrary.o -lm -pthread 
	@echo "... executing program ..."
	@echo 
	@./exe01 ${N}

clean: 
	@clear
	@rm -f myLibrary.o	
	@rm -f solution.o
	@rm -f exe01
	@echo "... cleaner than your soul ..."