FILENAME := "exe01.txt"

target: 
	@clear
	@gcc solution.c -o exe01 -lcrypto
	@chmod +x exe01
	@./exe01 ${FILENAME}
	@rm exe01

clean:
	@clear
	@rm -f exe01
	@echo "All shiny..."