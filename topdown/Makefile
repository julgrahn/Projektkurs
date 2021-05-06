#Compile SDL project


theGame: main.o player.o world.o bullet.o net.o server.o
	gcc -o theGame main.o player.o world.o bullet.o net.o server.o -lSDL2 -lSDL2_image -lSDL2_net
	
main.o: main.c
	gcc -c main.c -g

player.o: player.c
	gcc -c player.c -g

world.o: world.c
	gcc -c world.c

bullet.o: bullet.c
	gcc -c bullet.c

server.o: server.c
	gcc -c server.c

net.o: net.c
	gcc -c net.c
