#Compile SDL project


theGame: main.o player.o
	gcc -o theGame main.o player.o -lSDL2 -lSDL2_image
	
main.o: main.c
	gcc -c main.c

player.o: player.c
	gcc -c player.c