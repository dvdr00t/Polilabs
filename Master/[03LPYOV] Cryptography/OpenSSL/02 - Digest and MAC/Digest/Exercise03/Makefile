FILENAME := "exe03.txt"
DGST_ALG := "sha512"

target: 
	@clear
	@gcc solution.c -o exe03 -lcrypto
	@chmod +x exe03
	@./exe03 ${FILENAME} ${DGST_ALG}
	@rm exe03

clean:
	@clear
	@rm -f exe03
	@echo "All shiny..."