#Compile SDL project


theGame: main.o player.o world.o
	gcc -o theGame main.o player.o world.o -lSDL2 -lSDL2_image -lSDL2_net
	
main.o: main.c
	gcc -c main.c

player.o: player.c
	gcc -c player.c

world.o: world.c
	gcc -c world.c