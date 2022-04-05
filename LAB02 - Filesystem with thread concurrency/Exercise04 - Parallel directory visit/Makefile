PATH_1 := "./source_directory_01"
PATH_2 := "./source_directory_02"
PATH_3 := "./source_directory_03"
PATH_4 := "./source_directory_04"

target:
	@clear
	@echo "[USAGE] make <solution_X>"


solution_A: 
	@clear
	@rm -f exe04
	@gcc solution_A.c -o exe04 -lpthread
	@chmod +x exe04
	@echo "... executing program ..."
	@echo 
	@./exe04 ${PATH_1} ${PATH_2} ${PATH_3} ${PATH_4}
	@sleep 2
	@echo "Done!"

solution_B:
	@clear
	@rm -f exe04
	@rm -f *_thread.txt
	@gcc solution_B.c -o exe04 -lpthread
	@chmod +x exe04
	@echo "... executing program ..."
	@echo 
	@./exe04 ${PATH_1} ${PATH_2} ${PATH_3} ${PATH_4}
	@sleep 2
	@echo "Done!"