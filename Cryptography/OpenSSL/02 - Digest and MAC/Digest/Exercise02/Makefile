FILENAME_A := "exe02_A.txt"
FILENAME_B := "exe02_B.txt"

target: 
	@clear
	@gcc solution.c -o exe02 -lcrypto
	@chmod +x exe02
	@./exe02 ${FILENAME_A} ${FILENAME_B}
	@rm exe02

clean:
	@clear
	@rm -f exe02
	@echo "All shiny..."