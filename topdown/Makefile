#Compile SDL project


theGame: main.o player.o world.o bullet.o net.o server.o networkgamestate.c networkplayer.c networkbullet.c
	gcc -o theGame main.o player.o world.o bullet.o net.o server.o networkgamestate.c networkplayer.c networkbullet.c -lSDL2 -lSDL2_image -lSDL2_net
	
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

net.o: net.c
	gcc -c net.c

gamestate.o: networkgamestate.c
	gcc -c gamestate.c

networkplayer.o: networkplayer.c
	gcc -c networkplayer.c

networkbullet.o: networkbullet.c
	gcc -c networkbullet.c