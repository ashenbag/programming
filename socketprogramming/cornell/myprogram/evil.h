#include <sys/types.h>
#include <sys/socket.h>

#define MAX_MSG_LEN 4096

/* The "evil.config" specifies the parameters used by this tool.
 * The following is an example of its format.
 *
 *     CRPT-PROB(%)  <probablity of corruption %>
 *     DROP-PROB(%)  <probablity of dropping %>
 *     DUP-PROB(%)   <probablity of duplicating %>
 *     DELAY-PROB(%) <probablity of delaying %>
 *     MIN-DELAY(ms) <minimum delay in milliseconds>
 *     MAX-DELAY(ms) <maximum delay in millisecnds>
 * 
 * The above format and order must not be changed.
 * The probabilities are in percentage (between 0 and 100).
 * Multiple operations can be set at the same time.
 * The actual delay is chosen randomly in the specified range.
 */

#define CONFIG_FILE "evil.config"


/* This function initiates the evil layer.
 * It reads the parameter from the config file.
 */

void evilInit ();

/* This function finishes the evil layer.
 * To be called at the end of program by the main thread.
 */

void evilQuit ();

/* This function finishes the evil layer for each thread.
 */

void evilThreadQuit ();


/* This function mimics an unreliable channel.
 * It introduces unreliability by performing the following operations:
 * Corruption: some byte of the msg is garbled.
 * Dropping: msg is dropped.
 * Delaying: msg is delayed by a fixed time.
 * Duplicating: a delayed duplicate of the msg is additionally sent.
 * It's parameters are similar to the socket sendto function.
 * It's return values are similar to the socket sendto function.
 * This function is non-blocking. The delaying is done asynchronously.
 */ 

ssize_t evilsendto (int sockfd, char *msg, size_t msgLen, int flags, struct sockaddr *toAddr, socklen_t addrLen);
