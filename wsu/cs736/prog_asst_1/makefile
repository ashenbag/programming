OBJS = server.o server_thread.o
CC = gcc
CFLAGS = -Wall -c
MLFLAGS = -Wall -lpthread -o

mtserver: $(OBJS)
	$(CC) $(MLFLAGS) mtserver $(OBJS)

server_thread.o: server.h server_thread.c
	$(CC) $(CFLAGS) server_thread.c

server.o: server.c server_thread.o
	$(CC) $(CFLAGS) server.c

clean:
	rm -rf *o mtserver
