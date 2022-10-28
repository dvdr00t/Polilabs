FILENAME := "exe03.txt"

target: 
	@clear
	@gcc solution.c -o exe03 -lcrypto
	@chmod +x exe03
	@./exe03 ${FILENAME}
	@rm exe03

clean:
	@clear
	@rm -f exe03
	@echo "All shiny..."