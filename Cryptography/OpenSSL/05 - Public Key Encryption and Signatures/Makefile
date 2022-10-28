FILE := "file.txt"

target:
	@clear
	@rm -f exe01
	@rm -f public.pem
	@rm -f private.pem
	@rm -f signature.bin
	@gcc solution.c -o exe01 -lcrypto
	@./exe01 ${FILE} ${KEY}

clean: 
	@clear
	@rm -f exe01
	@rm -f public.pem
	@rm -f private.pem
	@rm -f signature.bin
	@echo "All shiny..."
