OBJS = graph.o llist.o queue.o bfs.o mst.o
CC = gcc
CFLAGS = -Wall -Werror -g -c
MLFLAGS = -Wall -Werror -g -o

graph: $(OBJS)
	$(CC) $(MLFLAGS) graph $(OBJS)

llist.o: graph.h llist.c
	$(CC) $(CFLAGS) llist.c

queue.o: graph.h queue.c
	$(CC) $(CFLAGS) queue.c

bfs.o: graph.h bfs.c
	$(CC) $(CFLAGS) bfs.c

mst.o: graph.h mst.c
	$(CC) $(CFLAGS) mst.c

graph.o: graph.c llist.o queue.o bfs.o mst.o
	$(CC) $(CFLAGS) graph.c

clean:
	rm -rf *o graph
