#   VARIABLES
# -------------
PATH1 := "./dir1"
PATH2 := "./dir2"
PATH3 := "./dir3"
PATH4 := "./dir4"

target: 
	@clear
	@rm -f myLibrary.o
	@rm -f exe03
	@gcc -c myLibrary.c
	@gcc -o exe03 solution.c myLibrary.o -pthread
	@echo "... executing program ..."
	@echo 
	@./exe03 ${PATH1} ${PATH2} ${PATH3} ${PATH4}

clean: 
	@clear
	@rm -f myLibrary.o
	@rm -f exe03
	@echo "... look how beauty you are ..."