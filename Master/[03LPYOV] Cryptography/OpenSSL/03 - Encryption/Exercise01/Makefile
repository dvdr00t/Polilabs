FILENAME := "exe01.txt"
ENC_ALGO := "aes-128-cbc"

target: 
	@clear
	@gcc solution.c -o exe01 -lcrypto
	@chmod +x exe01
	@./exe01 ${FILENAME} ${ENC_ALGO}
	@rm exe01

clean:
	@clear
	@rm -f exe01
	@rm -f exe01.enc
	@echo "All shiny..."