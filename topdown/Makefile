#Compile SDL project


theGame: main.o player.o world.o bullet.o
	gcc -o theGame main.o player.o world.o bullet.o -lSDL2 -lSDL2_image
	
main.o: main.c
	gcc -c main.c

player.o: player.c
	gcc -c player.c

world.o: world.c
	gcc -c world.c

bullet.o: bullet.c
	gcc -c bullet.c