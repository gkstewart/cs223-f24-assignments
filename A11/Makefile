CC = gcc
FLAGS = -g -Wall -Wvla -Werror

all: grep

grep: grep.c
	$(CC) $(FLAGS) grep.c -o grep -lpthread

clean:
	rm -rf grep
