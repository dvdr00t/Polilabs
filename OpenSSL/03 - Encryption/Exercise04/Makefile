MESSAGE := "This message knows no pad"

target: 
	@clear
	@gcc solution.c -o exe04 -lcrypto
	@chmod +x exe04
	@./exe04 ${MESSAGE}
	@rm exe04

clean:
	@clear
	@rm -f exe04
	@echo "All shiny..."