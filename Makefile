all:
	gcc -Wall -O3 -o ./mandelbrot ./src/mandelbrot.c -lm -lncurses

run: all
	./mandelbrot
