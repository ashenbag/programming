#include "utcp.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* This server merely echos the received messages. */

//extern int errno;

struct packet {
  connIdType connId;
  int dataLen;
  char data[MAX_DATA_LEN];
};

void *echoServer (void *args) {
  int connId = (int) args;
  struct packet pkt;

  while ((pkt.dataLen = utcpRecv(connId, pkt.data, MAX_DATA_LEN)) >= 0) {
    if (utcpSend(connId, pkt.data, pkt.dataLen) < 0) {
      printf("ERROR: %d in utcpSend.\n", errno);
      utcpClose(connId);
    }
  }
  printf("ERROR: %d in utcpSend.\n", errno);
  utcpClose(connId);
  utcpThreadQuit();
}

main(int argc, char *argv[]) {
  connIdType connId;
  bindIdType bindId;

  char data[MAX_DATA_LEN];
  int dataLen;
  int port;
  struct packet *pkt;
  
  int windowSize = 1024;
  
  if (argc != 2) {
    printf("USAGE: sampleServer <port #>.\n");
    exit(0);
  }
  port = atoi(argv[1]);

  utcpInit();

  if ((bindId = utcpBind(port)) < 0) {
    utcpQuit();
    printf("ERROR: %d.\n", errno);
    exit(0);
  }
  
  while ((connId = utcpAccept(bindId, windowSize)) >= 0) {
    pthread_t threadid;

    if (pthread_create(&threadid, NULL, echoServer, (char *)connId) < 0) {
      printf("ERROR: %d in pthread_create.\n", errno);
      utcpClose(connId);
      utcpQuit();
      exit(0);
    }
  }
  
  printf("ERROR: %d.\n", errno);
  utcpQuit();
  exit(0);
}









