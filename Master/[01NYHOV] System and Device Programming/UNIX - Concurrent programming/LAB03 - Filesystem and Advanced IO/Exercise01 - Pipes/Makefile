WAIT_TIME_1 := 3
WAIT_TIME_2 := 4
STR_NUM     := 2
STR_SIZE    := 16

solution_MULTITHREADING:
	@clear
	@rm -f exe_MULTITHREADING
	@gcc -pthread -Wall solution_MULTITHREADING.c -o exe_MULTITHREADING
	@chmod +x exe_MULTITHREADING
	@echo "... executing program ..."
	@./exe_MULTITHREADING ${WAIT_TIME_1} ${WAIT_TIME_2} ${STR_NUM} ${STR_SIZE}

solution_NON-BLOCKING:
	@clear
	@rm -f exe_NON-BLOCKING
	@gcc -Wall solution_NON-BLOCKING.c -o exe_NON-BLOCKING
	@chmod +x exe_NON-BLOCKING
	@echo "... executing program ..."
	@./exe_NON-BLOCKING ${WAIT_TIME_1} ${WAIT_TIME_2} ${STR_NUM} ${STR_SIZE}

solution_MULTIPLEXING:
	@clear
	@rm -f exe_MULTIPLEXING
	@gcc -Wall solution_MULTIPLEXING.c -o exe_MULTIPLEXING
	@chmod +x exe_MULTIPLEXING
	@echo "... executing program ..."
	@./exe_MULTIPLEXING ${WAIT_TIME_1} ${WAIT_TIME_2} ${STR_NUM} ${STR_SIZE}

solution_ASYNCHRONOUS:
	@clear
	@rm -f exe_ASYNCHRONOUS
	@gcc -Wall solution_ASYNCHRONOUS.c -o exe_ASYNCHRONOUS -lrt
	@chmod +x exe_ASYNCHRONOUS
	@echo "... executing program ..."
	@./exe_ASYNCHRONOUS ${WAIT_TIME_1} ${WAIT_TIME_2} ${STR_NUM} ${STR_SIZE}
	