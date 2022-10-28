#   VARIABLES
# -------------
F1   := "f1.bin"
F2   := "f2.bin"
F3   := "f3.bin"
FOUT := "out.bin"

target: 
	@clear
	@rm -f myLibrary.o
	@rm -f exe02
	@rm -f out.bin
	@gcc -c myLibrary.c
	@gcc -o exe02 solution.c myLibrary.o -pthread
	@echo "... executing program ..."
	@echo 
	@./exe02 ${F1} ${F2} ${F3} ${FOUT}

clean: 
	@clear
	@rm -f myLibrary.o
	@rm -f exe02
	@rm -f out.bin
	@echo "... all shiny ..."