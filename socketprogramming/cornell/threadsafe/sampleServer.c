#include "utcp.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

/* This server merely echos the received messages. */

//extern int errno;

struct packet {
  connIdType connId;
  int dataLen;
  char data[MAX_DATA_LEN];
};

void *echoMsg (void *args) {
  struct packet *pkt = (struct packet *) args;

  if (utcpSend(pkt->connId, pkt->data, pkt->dataLen) < 0) {
    printf("echoMsgERROR: %d.\n", errno);
    utcpClose(pkt->connId);
  }

  free(pkt);
  utcpThreadQuit();
  return NULL;
}

int main(int argc, char *argv[]) {
  connIdType connId;
  bindIdType bindId;

  //char data[MAX_DATA_LEN];
  //int dataLen;
  int port;
  struct packet *pkt;
  
  int windowSize = 1024;

  if (argc != 2) {
    printf("USAGE: sampleServer <port #>.\n");
    return 0;
  }
  port = atoi(argv[1]);

  utcpInit();

  if ((bindId = utcpBind(port)) < 0) {
    utcpQuit();
    printf("mainbindERROR: %d.\n", errno);
    return 0;
  }
  
  if ((connId = utcpAccept(bindId, windowSize)) < 0) {
    utcpQuit();
    printf("mainacceptERROR: %d.\n", errno);
    return 0;
  }
  
  pkt = (struct packet *)malloc(sizeof(struct packet));
  assert(pkt != NULL);

  while ((pkt->dataLen = utcpRecv(connId, pkt->data, MAX_DATA_LEN)) >= 0) {
    pthread_t threadid;

    pkt->connId = connId;
    if (pthread_create(&threadid, NULL, echoMsg, pkt) < 0) {
      printf("ERROR: %d in pthread_create.\n", errno);
      utcpClose(connId);
      utcpQuit();
      return 0;
    }
    pkt = (struct packet *)malloc(sizeof(struct packet));
    assert(pkt != NULL);
  }
  
  printf("afterwhileERROR: %d.\n", errno);
  utcpClose(connId);
  utcpQuit();
  return 0;
}









