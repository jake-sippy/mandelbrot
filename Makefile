CC=g++
CFLAGS=-g -Wall
PROGS=app

all: $(PROGS)

mandelbrot.o: mandelbrot.cc
	$(CC) $(CFLAGS) -c $<

app: mandelbrot.o
	$(CC) $(CFLAGS) -o $@ $< -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f *.o app
