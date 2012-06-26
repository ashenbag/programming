#include "server.h"

unsigned short int gcount = 0; /* Variable to maintain number of threads. */

/* Main program. 
   1. Creates socket.
   2. Bind to address and port number.
   3. Listens on the created socket.
   4. It checks if Maximum client connection is not exceeded.
   5. When a client connects, a new thread is created for the client. Socket information is passed from main program to thread.
   6. Main program continues to listen for ever.
 */

int main()
{
	int listen_socket,rc,thread_search=0;
	struct sockaddr_in server_address;
	struct thread_info tinfo[MAX_CONNECTIONS];
	socklen_t sin_size;

	/* Creating socket */
	listen_socket = socket(AF_INET,SOCK_STREAM,0);
	if(listen_socket == -1)
	{
		perror("\nServer Socket creation error.");
		return -1;
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY; /* Listen to any IP address in the system. */
	bzero(&(server_address.sin_zero),8);

	rc = bind(listen_socket,(struct sockaddr *)&server_address,sizeof(struct sockaddr));
	if(rc == -1)
	{
		perror("\nServer Bind error.");
		return -1;
	}

	rc = listen(listen_socket,10);
	if(rc == -1)
	{
		perror("\nServer Listen error.");
		return -1;
	}

	while(TRUE)
	{
		sin_size = sizeof(struct sockaddr_in);
		while(tinfo[gcount].flag!=0)
		{
			/* Maintaing number of threads in the application.
			   It looks for free threads by checking the flag in thread structure.
			   If it is not set then the thread instance is free and a thread is created. 
			   If search is more than times MAX_CONNECTIONS, message is displayed. */
			gcount++;
			if(gcount>MAX_CONNECTIONS)
			{
				gcount=0;
			}
			if(thread_search>MAX_CONNECTIONS)
			{
				printf("\nMaximum simultaneous connections exceeded. No more connections possible.");
				thread_search=0;
			}

		}
		tinfo[gcount].connection_socket = accept(listen_socket,(struct sockaddr *)&(tinfo[gcount].client_address),&sin_size);
		if(tinfo[gcount].connection_socket == -1)
		{
			perror("\nServer Accept error.");
			continue;
		}
		/* Creating thread. */
		rc = pthread_create(&tinfo[gcount].thread_id,NULL,(void *) &threadFunction,(void*)&tinfo[gcount]);
		if(rc != 0)
		{
			perror("\nServer pthread_create failed.");
			continue;
		}
	}
	close(listen_socket);

	return 0;
}
