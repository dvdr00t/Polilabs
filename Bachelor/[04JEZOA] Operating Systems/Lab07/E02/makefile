N=3
files_IN=file1_in.txt file2_in.txt file3_in.txt
files_OUT=file1_out.txt file2_out.txt file3_out.txt

target:
	@clear
	@gcc -Wall -o test script_L07E02.c
	@./test $(N) $(files_IN) $(files_OUT)
	@echo "file1_out: "
	@cat file1_out.txt
	@echo
	@echo "file2_out: "
	@cat file2_out.txt
	@echo
	@echo "file3_out: "
	@cat file3_out.txt
	@echo
	@rm $(files_OUT)	