N=3
files_IN=file1_in.txt file2_in.txt file3_in.txt
files_OUT=file_out.txt

target:
	@clear
	@gcc -Wall -o test script_L07E03.c
	@./test $(N) $(files_IN) $(files_OUT)
	@echo "file_out: "
	@cat file_out.txt
	@echo
	@rm $(files_OUT)	

