CC=gcc
CFLAGS=-g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable
LDFLAGS=-lpthread

all: thread_mandelbrot

thread_mandelbrot: thread_mandelbrot.c read_ppm.c write_ppm.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f thread_mandelbrot
