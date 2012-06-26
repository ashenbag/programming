#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "utcp.h"

/* This client merely sends key board input to server and prints response. */

//extern int errno;

main(int argc, char *argv[]) {
  connIdType connId;

  int numData;

  char data[MAX_DATA_LEN];
  int dataLen;
  int port;
  int windowSize = 1024;

  if (argc != 4) {
    printf("USAGE: sampleClient <IP address> <port #> <# data>.\n");
    exit(0);
  }
  port = atoi(argv[2]);
  numData = atoi(argv[3]);
  
  utcpInit();

  if ((connId = utcpConnect(argv[1], port, windowSize)) < 0) {
    printf("ERROR: %d.\n", errno);
    exit(0);
  }

  for (; numData>0 ; numData--) {
    scanf("%s", data);

    if (utcpSend(connId, data, strlen(data)) < 0) {
      printf("ERROR: %d.\n", errno);
      utcpClose(connId);
      exit(0);
    }
    
    memset(data, 0, MAX_DATA_LEN);

    if ((dataLen = utcpRecv(connId, data, MAX_DATA_LEN)) < 0) {
      printf("ERROR: %d.\n", errno);
      utcpClose(connId);
      exit(0);
    }

    printf("%s\n",data);
  }
  
  utcpClose(connId);
  utcpQuit();
  exit(0);
}


