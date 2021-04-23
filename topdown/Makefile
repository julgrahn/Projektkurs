#Compile SDL project


theGame: main.o player.o world.o net.o
	gcc -o theGame main.o player.o world.o net.o -lSDL2 -lSDL2_image -lSDL2_net
	
main.o: main.c
	gcc -c main.c

player.o: player.c
	gcc -c player.c

world.o: world.c
	gcc -c world.c

net.o: net.c
	gcc -c net.c