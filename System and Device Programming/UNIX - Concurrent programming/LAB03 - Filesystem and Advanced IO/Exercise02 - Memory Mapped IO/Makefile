FILENAME := "file_input"

target: 
	@clear
	@rm -f exe02
	@gcc -pthread -Wall solution_memory-mapped.c -o exe02
	@chmod +x exe02
	@echo "... executing program ..."
	@./exe02 file_input

reset:
	@clear
	@rm -f exe02
	@rm -f file_input
	@cp ./file_input_ORIGINAL ./file_input
	@gcc -pthread -Wall solution_memory-mapped.c -o exe02
	@chmod +x exe02
	@echo "... executing program ..."
	@./exe02 file_input