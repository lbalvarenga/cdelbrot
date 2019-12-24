all:
	gcc -Wall -O3 -lm -lncurses -o ./mandelbrot ./src/mandelbrot.c

run: all
	./mandelbrot