all : run

run : build
	./bruh

build : snek.o clientAPI.o snakeAPI.o
	gcc -o bruh snek.o clientAPI.o snakeAPI.o

snek.o : snek.c
	gcc -o snek.o -c snek.c

clientAPI.o : clientAPI.c snakeAPI.o
	gcc -o clientAPI.o -c clientAPI.c

snakeAPI.o : snakeAPI.c
	gcc -o snakeAPI.o -c snakeAPI.c

clean : 
	rm -f *.o
	rm bruh