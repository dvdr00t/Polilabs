FILENAME_IN := "exe01.enc"
FILENAME_OUT := "exe02.txt"
KEY := "aabd214f3fe8f03453885123a8318f24"
IV := "7c4e5b074c0c7c8579e38cbc51b4d4a6"

target: 
	@clear
	@gcc solution.c -o exe02 -lcrypto
	@chmod +x exe02
	@./exe02 ${FILENAME_IN} ${KEY} ${IV} ${FILENAME_OUT}
	@rm exe02

clean:
	@clear
	@rm -f exe02
	@rm -f exe02.txt
	@echo "All shiny..."