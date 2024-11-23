CC=gcc
<<<<<<< HEAD
CFLAGS=-g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS=-lpthread
=======
SOURCES=thread_mandelbrot single_mandelbrot
FILES := $(subst .c,,$(SOURCES))
FLAGS=-g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable
>>>>>>> upstream/master

all: thread_mandelbrot

thread_mandelbrot: thread_mandelbrot.c read_ppm.c write_ppm.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f thread_mandelbrot
