#include "server.h"

extern unsigned short int gcount;

/* Thread function. 
   1. Receives HTTP request from client and stores in buffer.
   2. Extracts the file information and checks for its availability.
   3. If the requested file does not exists "File not found" response is send back.
   4. If the requested file exists, it reads the file, constructs the HTTP header and sends the file along with the HTTP header.
   5. In the end, pthread_exit() is called to terminate the thread.
  */ 

void threadFunction(void *param)
{
	struct thread_info *tinfo = (struct thread_info *)param;
	FILE *fp;

	char delimiter[] = "\r\n";
	char *data_in,*data_out;
	char *endstring, *tempstring, *tokstring, *buffer;
	char *file_filename;
	int connection_socket,sr_return,tempstringlength,rc,length;
	long int file_filesize;
	ssize_t file_readbytes;

	char *http_statusline;
	char *http_contenttypeline;
	char *http_contentlengthline;
	char *http_connection;
	char *http_entitybody;

	connection_socket = tinfo->connection_socket;

	data_in = (char*)malloc(MAX_DATA_SIZE);
	data_out = (char*)malloc(MAX_DATA_SIZE);
	tempstring = (char*)malloc(MAX_DATA_SIZE/4);
	http_contentlengthline = (char*)malloc(MAX_DATA_SIZE/4);
	do
	{
		/* Receiving data from client. This is a non blocking recv() system call.*/
		sr_return = recv(connection_socket,data_in,MAX_DATA_SIZE,MSG_DONTWAIT);
		if(sr_return == -1)
		{
			if(errno != (EWOULDBLOCK || EAGAIN))
			{
				perror("\nServer receive error.");
				close(connection_socket);
				tinfo->flag = 0;
				pthread_exit(NULL);
			}
			else
			{
				/* Nothing has been read, so continue to read. */
				continue;
			}
		}
		else
		{
			/* If something is read, go ahead and process it. */
			break;
		}
	}while(TRUE);

	/* Extracting filename from the HTTP Request message header line. */
	endstring = strstr(data_in,delimiter);
	tempstringlength = endstring-data_in;
	strncpy(tempstring,data_in,tempstringlength);
	tokstring = strtok(tempstring," /");
	file_filename = strtok(NULL," /");

	/* Reading the file. */
	fp = fopen(file_filename,"rb");
	if(fp == NULL)
	{
		/* FILE NOT FOUND Case. */

		/* Constructing HTTP header for response */
		http_statusline = "HTTP/1.0 404 Not Found\0";
		http_contenttypeline = gethttpcontenttype(".html");

		/* Non persistent HTTP. */
		http_connection = "Connection: close\0";
		http_entitybody = "<HTML><HEAD><TITLE> 404 Not Found</TITLE></HEAD><BODY>404: Object Not Found</BODY></HTML>\0";

		strcpy(data_out,http_statusline);
		strcat(data_out,delimiter);
		strcat(data_out,http_contenttypeline);
		strcat(data_out,delimiter);
		strcat(data_out,http_connection);
		strcat(data_out,delimiter);
		strcat(data_out,delimiter);
		strcat(data_out,http_entitybody);

		/* Sending HTTP Response message to client. */
		sr_return = send(connection_socket,data_out,strlen(data_out),0);
		if(sr_return == -1)
		{
			perror("\nServer Send error.");
			fclose(fp);
			close(connection_socket);
			tinfo->flag = 0;
			pthread_exit(NULL);
		}
		close(connection_socket);
		tinfo->flag = 0;
		pthread_exit(NULL);
	}
	else
	{
		/* Determining the file size */
		fseek(fp,0,SEEK_END);
		file_filesize = ftell(fp);
		rewind(fp);
		buffer = (char *)malloc(sizeof(char)*file_filesize);

		/* Reading file contents into buffer. */
		file_readbytes = fread(buffer,1,file_filesize,fp);
		if(file_readbytes != file_filesize)
		{
			printf("File read error.\n");
		}

		/* Constructing HTTP header. */
		http_statusline = "HTTP/1.0 200 OK\0";
		http_contenttypeline = gethttpcontenttype(file_filename);
		sprintf(http_contentlengthline,"%d",file_readbytes);
		/* Non-persistent HTTP */
		http_connection = "Connection: close\0";
		http_entitybody = buffer;

		strcpy(data_out,http_statusline);
		strcat(data_out,delimiter);
		strcat(data_out,http_connection);
		strcat(data_out,delimiter);
		strcat(data_out,"Content-length: ");
		strcat(data_out,http_contentlengthline);
		strcat(data_out,delimiter);
		strcat(data_out,http_contenttypeline);
		strcat(data_out,delimiter);
		strcat(data_out,delimiter);
		length = strlen(data_out);

		/* Sending HTTP Response - HTTP Header - to client. */
		sr_return = send(connection_socket,data_out,length,0);
		if(sr_return == -1)
		{
			perror("\nServer Send error.");
			fclose(fp);
			close(connection_socket);
			tinfo->flag = 0;
			pthread_exit(NULL);
		}
		
		/* Sending HTTP Response - File - to client. */
		sr_return = send(connection_socket,(const void *)http_entitybody,file_readbytes,0);
		if(sr_return == -1)
		{
			perror("\nServer Send error.");
			fclose(fp);
			close(connection_socket);
			tinfo->flag = 0;
			pthread_exit(NULL);
		}

		fclose(fp);
		rc = close(connection_socket);
		tinfo->flag = 0;
		fflush(stdout);
		pthread_exit(NULL);
	}
}

/* HTTP Content Type determination. 
   Based on the file extension, HTTP Response content type is return.
   */
char *gethttpcontenttype(char file_filename[])
{
	char *file_extension;
	file_extension = strrchr(file_filename,'.');
	if(((strcmp(file_extension,".html")) == 0) || ((strcmp(file_extension,".htm")) == 0))
	{
		return "Content-type: text/html";
	}
	if(((strcmp(file_extension,".jpg")) == 0) || ((strcmp(file_extension,".jpeg")) == 0))
	{
		return "Content-type: image/jpeg";
	}
	return "application/octet-stream";
}
