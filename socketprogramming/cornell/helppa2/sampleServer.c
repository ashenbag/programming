#include "utcp.h"

/* This server merely echos the received messages. */

extern int errno;

main(int argc, char *argv[]) {
  connIdType connId;
  bindIdType bindId;

  char data[MAX_DATA_LEN];
  int dataLen;
  int port;
  
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
  
  if ((connId = utcpAccept(bindId, windowSize)) < 0) {
    utcpQuit();
    printf("ERROR: %d.\n", errno);
    exit(0);
  }

  while ((dataLen = utcpRecv(connId, data, MAX_DATA_LEN)) >= 0) {
    if (utcpSend(connId, data, dataLen) < 0) {
      printf("ERROR: %d.\n", errno);
      utcpClose(connId);
      utcpQuit();
      exit(0);
    }
  }
  
  printf("ERROR: %d.\n", errno);
  utcpClose(connId);
  utcpQuit();
  exit(0);
}

