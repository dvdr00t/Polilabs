CC = gcc
FLAGS = -Wall

.PHONY: install clean distclean

target: script.o
	$(CC) $(FLAGS) -o myListFunction script.o

script.o: script.c
	$(CC) $(FLAGS) -c script.c

install: 
	mkdir ../bin;
	mv myListFunction ../bin;

clean:
	rm *.o

distclean: clean
	rm -R ../bin

