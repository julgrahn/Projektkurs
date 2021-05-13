#Compile SDL project


theGame: main.o player.o world.o bullet.o server.o networkgamestate.o initFunctions.o gameFunctions.o clientNetFunctions.o renderFunctions.o weapon.o button.o
	gcc -o theGame main.o player.o world.o bullet.o server.o networkgamestate.o initFunctions.o gameFunctions.o clientNetFunctions.o renderFunctions.o weapon.o button.o -lSDL2 -lSDL2_image -lSDL2_net -lSDL2_mixer -lSDL2_ttf
	
main.o: main.c
	gcc -c main.c

player.o: player.c
	gcc -c player.c

world.o: world.c
	gcc -c world.c

bullet.o: bullet.c
	gcc -c bullet.c

server.o: server.c
	gcc -c server.c

networkgamestate.o: networkgamestate.c
	gcc -c networkgamestate.c

initFunctions.o: initFunctions.c
	gcc -c initFunctions.c

gameFunctions.o: gameFunctions.c
	gcc -c gameFunctions.c

clientNetFunctions.o: clientNetFunctions.c
	gcc -c clientNetFunctions.c

renderFunctions.o: renderFunctions.c
	gcc -c renderFunctions.c

weapon.o: weapon.c
	gcc -c weapon.c

button.o: button.c
	gcc -c button.c
