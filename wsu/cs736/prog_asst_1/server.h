#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <pthread.h>

#define TRUE 1
#define SERVER_PORT 50505
#define MAX_DATA_SIZE 65535
#define MAX_CONNECTIONS 10 /* Allowing upto 10 simulatenous connections. */

void threadFunction(void *);
char *gethttpcontenttype(char *);

/* Information passed from main program to thread. */
struct thread_info
{
	int flag; /* Indicating if thread is currently active.*/
	int connection_socket;
	pthread_t thread_id;
	struct sockaddr_in client_address;
};
