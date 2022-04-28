PATH_1 := "./source_directory"
PATH_2 := "./destination_directory"

target: 
	@clear
	@rm -f exe03
	@rm -rf ${PATH_2}
	@gcc solution.c -o exe03
	@chmod +x exe03
	@echo "... executing program ..."
	@echo 
	@./exe03 ${PATH_1} ${PATH_2}
	@sleep 2
	@echo "Done!"