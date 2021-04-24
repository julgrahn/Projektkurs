#Compile SDL project


theGame: main.o player.o world.o net.o
	gcc -o theGame main.o player.o world.o net.o -lSDL2 -lSDL2_image -lSDL2_net -g
	
main.o: main.c
	gcc -c main.c -g

player.o: player.c
	gcc -c player.c -g

world.o: world.c
	gcc -c world.c -g

net.o: net.c
	gcc -c net.c -g