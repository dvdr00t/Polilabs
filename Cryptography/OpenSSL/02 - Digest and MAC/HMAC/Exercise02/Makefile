FILENAME := "exe02.txt"
HMAC_VAL := "dc2b1b6e03f97529d3418797c4bb6fd85392939c738918103504c8b26d38ea25"

target: 
	@clear
	@gcc solution.c -o exe02 -lcrypto
	@chmod +x exe02
	@./exe02 ${HMAC_VAL} ${FILENAME}
	@rm exe02

clean:
	@clear
	@rm -f exe02
	@echo "All shiny..."