#include "headers.h"
#include "utcp.h"
#include "evil.h"
#include "eventTimer.h"
#include "cksum.h"

#define MAX_CONNECTIONS 1 /* only one connection accepted. */

#define DEBUG

/* The following helps print debug statements optionally. */
#ifdef DEBUG
#define PRINTDEBUG(x) x
#else
#define PRINTDEBUG(x) 
#endif

//extern int errno;

/* Data structure used to keep track of multiple connections. */
/* You may add additional fields to this structure. */
typedef struct {
  int sockfd;
  struct sockaddr_in addr;
} connectionType;

connectionType connection[MAX_CONNECTIONS];
int numConns = 0;

void utcpInit () {
}

void utcpQuit () {
}

void utcpThreadQuit () {
  evilThreadQuit();
}

bindIdType utcpBind (int port) {
  int sockfd;
  struct sockaddr_in serverAddr;

  evilInit();

  /* Open a UDP socket. */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    return -1;
  }

  /* Bind the local address to that socket. */
  bzero((char *) &serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(port);

  if (bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
    return -1;
  }

  PRINTDEBUG(printf("Binded socket %d to local port %d.\n", sockfd, port));

  return  sockfd;
}


connIdType utcpConnect (char *ipAddr, int port, int windowSize) {
  int sockfd;
  struct sockaddr_in clientAddr;

  if (numConns == MAX_CONNECTIONS) {
    errno = EMAXCONN;
    return -1;
  }

  evilInit();

  /* Open a UDP socket. */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    return -1;
  }

  /* Bind any local address to that socket. */
  bzero((char *) &clientAddr, sizeof(clientAddr));
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  clientAddr.sin_port = htons(0);

  if (bind(sockfd, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) < 0) {
    return -1;
  }

  connection[numConns].sockfd = sockfd;
  /* Fill in the address information of the server. */
  bzero((char *) &connection[numConns].addr, sizeof(connection[numConns].addr));
  connection[numConns].addr.sin_family = AF_INET;
  connection[numConns].addr.sin_addr.s_addr = inet_addr(ipAddr);
  connection[numConns].addr.sin_port = htons(port);
  numConns++;

  PRINTDEBUG(printf("Connected to server %s at port %d.\n", ipAddr, port)); 
  PRINTDEBUG(printf("ConnId %d: remote addr %s remote port %d  on  socket %d.\n", numConns-1, inet_ntoa(connection[numConns-1].addr.sin_addr), ntohs(connection[numConns-1].addr.sin_port), connection[numConns-1].sockfd));

  /* The following sends a dummy message. */
  /* You may have to send explicit control messages instead. */
  /* You may also need to initialize other data structures. */ 

  utcpSend(numConns-1, "CONNECT", sizeof("CONNECT"));

  /* You may need to receive messages asynchronously. */

  return  numConns-1;
}

connIdType utcpAccept (bindIdType bindId, int windowSize) {
  UTCPPacketType utcpPkt;
  //int addrLen;
  socklen_t addrLen;

  if (numConns == MAX_CONNECTIONS) {
    errno = EMAXCONN;
    return -1;
  }

  /* The following receives a dummy message. */
  /* You may have to receive and reply explicit control messages instead. */
  /* You may also need to initialize other data structures. */ 

  addrLen = sizeof(connection[numConns].addr);
  if (recvfrom(bindId, (char *) &utcpPkt, MAX_MSG_LEN, 0, (struct sockaddr *) &connection[numConns].addr, &addrLen) < 0) {
    return -1;
  }

  connection[numConns].sockfd = bindId;
  numConns++;

  PRINTDEBUG(printf("Accepted connection from client.\n"));
  PRINTDEBUG(printf("ConnId %d: remote addr %s remote port %d  on  socket %d.\n", numConns-1, inet_ntoa(connection[numConns-1].addr.sin_addr), ntohs(connection[numConns-1].addr.sin_port), connection[numConns-1].sockfd));

  return  numConns-1;  
}

int utcpClose (connIdType connId) {

  /* The following just closes the socket. */
  /* You may have to send and handle explicit control messages instead. */
  /* You may also need to finalize other data structures. */ 

  if ((connId < 0) || (connId >= numConns)) {
    errno = EINVAL;
    return -1;
  } 

  evilQuit();

  if (close(connection[connId].sockfd) < 0) {
    return -1;
  }

  PRINTDEBUG(printf("Closing connection on socket %d.\n", connection[connId].sockfd));

  return 0;
}

int utcpSend (connIdType connId, char *data, int dataLen) {
  UTCPPacketType utcpPkt;
  int pktSize;
  int bytesSent;
  int checkSum;

  char replydata[MAX_DATA_LEN];
  int ret;

  if ((connId < 0) || (connId >= numConns)) {
    errno = EINVAL;
    return -1;
  } 

  /* The following just sends the socket. */
  /* You may have to buffer the message and set retransmission timers. */
  /* You may also need to update the header and other data structures. */ 

  utcpPkt.header.dataLen = dataLen;
  memcpy(utcpPkt.data, data, dataLen);
  if((strcmp(data,"ACK")) != 0) {
	  utcpPkt.header.flags = 0; //indicating data
  } else {
	  utcpPkt.header.flags = 1; //indicating ACK
  }
  pktSize = sizeof(utcpPkt.header) + dataLen;

  /* Computing checksum and updating header */
  utcpPkt.header.checkSum = 0;
  checkSum = in_cksum(&utcpPkt,pktSize);//computing checksum for data only
  utcpPkt.header.checkSum = checkSum;
  PRINTDEBUG(printf("utcpSend checkSum %d.\n", checkSum));

  /* In the presence of signals, the send must be an atomic operatons. */
  /* You may need to block and unblock signals. */

  bytesSent = evilsendto(connection[connId].sockfd, (char *) &utcpPkt, pktSize, 0, (struct sockaddr *) &connection[connId].addr, sizeof(connection[connId].addr));

  if (bytesSent < 0) {
    return -1;
  }
  
  PRINTDEBUG(printf("Sent packet: remote addr %s remote port %d  on  socket %d.\n", inet_ntoa(connection[connId].addr.sin_addr), ntohs(connection[connId].addr.sin_port), connection[connId].sockfd));

  printf("aaa%s",data);
  if((ret = strcmp(data,"ACK")) == 0) {
	  PRINTDEBUG(printf("Sent ACK"));
  } else {
	  //Sent data so expecting ack
    	memset(replydata, 0, MAX_DATA_LEN);
	  PRINTDEBUG(printf("Sent data waiting for ACK."));
	  if ((dataLen = utcpRecv(connId, replydata, MAX_DATA_LEN)) < 0) {
		  printf("ERROR: %d.\n", errno);
		  utcpClose(connId);
		  return 0;
	  }
	  printf("ddd%s",replydata);
	  if((strcmp(replydata,"ACK")) == 0) {
		  PRINTDEBUG(printf("Successfully received ACK."));
		  return 0;
	  } else {
		  PRINTDEBUG(printf("Did not receive ACK. Resending..."));
		  if (utcpSend(connId, data, strlen(data)) < 0) {
			  printf("ERROR: %d.\n", errno);
			  utcpClose(connId);
			  return 0;
		  }
	  }
  }

  return 0; 
}


int utcpRecv (connIdType connId, char *data, int dataLen) {
  UTCPPacketType utcpPkt;
  //int pktSize;
  int bytesRcvd;
  //int addrLen;
  socklen_t addrLen;
  int checkSum;

  struct sockaddr_in addr;

  if ((connId < 0) || (connId >= numConns)) {
    errno = EINVAL;
    return -1;
  } 

  /* The following is completely uselees for utcp implementation. */
  /* You may need to receive messages asynchronously and buffer it instead. */
  /* Here you would probably just wait and copy data from a buffer. */ 

  addrLen = sizeof(addr);
  if ((bytesRcvd = recvfrom(connection[connId].sockfd, (char *) &utcpPkt, MAX_MSG_LEN, 0, (struct sockaddr *) &addr, &addrLen)) < 0) {
    return -1;
  }

  PRINTDEBUG(printf("Rcvd packet: size %d remote addr %s remote port %d  on  socket %d.\n", bytesRcvd, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port), connection[connId].sockfd));

  if (bytesRcvd < sizeof(UTCPHeaderType)) {
    return -1;
  }

  if (bytesRcvd < (sizeof(UTCPHeaderType) + utcpPkt.header.dataLen)) {
    return -1;
  }
  
  /* Computing and checking checksum*/
  checkSum = utcpPkt.header.checkSum;
  utcpPkt.header.checkSum = 0;
  PRINTDEBUG(printf("utcpRecv checkSuminHdr %d calculatedcksum %d.\n",checkSum,in_cksum(&utcpPkt,(utcpPkt.header.dataLen + sizeof(utcpPkt.header)))));
  if(checkSum != in_cksum(&utcpPkt,(utcpPkt.header.dataLen + sizeof(utcpPkt.header)))) {
	  printf("Corrupted: utcpRecv checkSuminHdr %d calculatedcksum %d.\n",checkSum,in_cksum(&utcpPkt,(utcpPkt.header.dataLen + sizeof(utcpPkt.header))));
  }
  if(utcpPkt.header.flags == 0) {
	  PRINTDEBUG(printf("Received Data. Sending ACK"));
	  if (utcpSend(connId, "ACK", strlen("ACK")) < 0) {
		  printf("ERROR: %d.\n", errno);
		  utcpClose(connId);
		  return 0;
	  }

  } else {
	  PRINTDEBUG(printf("Received ACK."));
  }
  
  memcpy(data, utcpPkt.data, utcpPkt.header.dataLen);

  return utcpPkt.header.dataLen;
}
