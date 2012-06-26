#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <sys/time.h>
#include <signal.h>
#include <asm/errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "eventTimer.h"

#define TIMECMP(a,b,cmp) (((a).tv_sec cmp (b).tv_sec) || (((a).tv_sec == (b).tv_sec) && ((a).tv_usec cmp (b).tv_usec)))

//#define DEBUG
#ifdef DEBUG
#define PRINTDEBUG(x) x
#else
#define PRINTDEBUG(x) 
#endif

extern int errno;

typedef struct EE {
  struct timeval time;
  void (*handler)(void *);
  void *args;
  int argLen;
  struct EE *next;
} eventEntryType;

eventEntryType eventQ = {{0,0}, NULL, NULL};
int msgqid;

int insertEventQ (struct timeval time, void (*handler)(void *), void *args, int argLen) {
  eventEntryType *new, *cur;
  int pos;

  new = (eventEntryType *)malloc(sizeof(eventEntryType));
  assert(new != NULL);

  new->time.tv_sec = time.tv_sec;
  new->time.tv_usec = time.tv_usec;
  new->handler = handler;
  if (argLen > 0) {
    new->args = malloc(argLen);
    assert (new->args != NULL);
    memcpy(new->args, args, argLen);
  }
  else {
    new->args = NULL;
  }
  new->argLen = argLen;

  for (cur = &eventQ, pos = 0; (cur->next != NULL) && (TIMECMP(cur->next->time, time, <)); cur = cur->next, pos++);

  new->next = cur->next;
  cur->next = new;

  return pos;
}

void resetTimer (struct timeval time) {
  struct itimerval value = {{0,0}, {0,0}};

  if (eventQ.next == NULL) {
    return;
  }

  assert(TIMECMP(time, eventQ.next->time, <));
  
  value.it_value.tv_sec = eventQ.next->time.tv_sec - time.tv_sec;
  value.it_value.tv_sec -= (eventQ.next->time.tv_usec < time.tv_usec) ? 1 : 0;
  value.it_value.tv_usec = (eventQ.next->time.tv_usec < time.tv_usec) ? (eventQ.next->time.tv_usec + 1000000 - time.tv_usec) : (eventQ.next->time.tv_usec - time.tv_usec);

  if (setitimer(ITIMER_REAL, &value, 0) < 0) {
    fprintf(stderr, "Error: %d in event timer: setitimer.\n", errno);
    exit(0);
  }
}

void handleSigAlrm (int signo) {
  eventEntryType *temp;
  void (*handler)(void *);
  void *args;
  struct timeval time;
  int mask;

  if (eventQ.next == NULL) {
    return;
  }

  mask = sigblock(sigmask(SIGIO));

  PRINTDEBUG(printf("EventTimer: handling SIGALRM.\n"));

  if (gettimeofday(&time, 0) < 0) {
    fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
    exit(0);
  }

  while (!TIMECMP(eventQ.next->time, time, >)) {
    temp = eventQ.next;
    eventQ.next = temp->next;
    handler = temp->handler;
    args = temp->args;
    free(temp);
  
    sigsetmask(mask);

    handler(args);

    if (args != NULL) {
      free(args);
    }

    if (eventQ.next == NULL) {
      return;
    }

    mask = sigblock(sigmask(SIGIO));

    if (gettimeofday(&time, 0) < 0) {
      fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
      exit(0);
    }
  }

  resetTimer(time);
  sigsetmask(mask);
}

int setEvent (struct timeval delay, void (*handler)(void *), void *args, int argLen) {
  struct timeval time, curTime;
  int mask;

  if (handler == NULL || delay.tv_usec > 1000000 || delay.tv_sec < 0 || delay.tv_usec < 0 || (delay.tv_sec == 0 && delay.tv_usec == 0) || argLen < 0 || (argLen > 0 && args == NULL)) {
    errno = EINVAL;
    return -1;
  }

  mask = sigblock(sigmask(SIGALRM)|sigmask(SIGIO));

  if (gettimeofday(&curTime, 0) < 0) {
    fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
    exit(0);
  }

  time.tv_sec = curTime.tv_sec + delay.tv_sec + (curTime.tv_usec + delay.tv_usec)/(long)1000000;
  time.tv_usec = (curTime.tv_usec + delay.tv_usec)%(long)1000000;
  
  if (insertEventQ(time, handler, args, argLen) == 0) {
    resetTimer(curTime);
  }

  sigsetmask(mask);
  return 0;
}

void eventTimerInit () {
  signal(SIGALRM, handleSigAlrm);
}

void eventTimerQuit () {
  eventEntryType *cur;
  int mask;
  struct timeval time, delay;

  if (eventQ.next == NULL) {
    return;
  }

  for (cur = eventQ.next; cur->next != NULL; cur = cur->next);

  if (gettimeofday(&time, 0) < 0) {
    fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
    exit(0);
  }

  delay.tv_sec = (time.tv_sec > cur->time.tv_sec) ? 1 : (cur->time.tv_sec - time.tv_sec + 3);
  delay.tv_usec = 0;

  PRINTDEBUG(printf("EventTimer: waiting to quit.\n"));

  while (select(FD_SETSIZE, NULL, NULL, NULL, &delay) < 0) {
    if (errno != EINTR) {
      fprintf(stderr, "Error: %d in event timer: select.\n", errno);
      exit(0);
    }
  }

  PRINTDEBUG(printf("EventTimer: quitting.\n"));
}

/*
void testEvent (void *num) {
  printf ("event %d.\n", *(int *)num);
}

main () {
  int arg;

  eventTimerInit();

  arg = 1;
  setEvent((struct timeval){10,0}, testEvent, 
&arg, sizeof(int));
  arg = 2;
  setEvent((struct timeval){8,0}, testEvent, &arg, sizeof(int));
  arg = 3;
  setEvent((struct timeval){9,0}, testEvent, &arg, sizeof(int));
  arg = 4;
  setEvent((struct timeval){6,0}, testEvent, &arg, sizeof(int));
  arg = 5;
  setEvent((struct timeval){7,0}, testEvent, &arg, sizeof(int));
  arg = 6;
  setEvent((struct timeval){5,0}, testEvent, &arg, sizeof(int));
  arg = 7;
  setEvent((struct timeval){3,0}, testEvent, &arg, sizeof(int));
  arg = 8;
  setEvent((struct timeval){4,0}, testEvent, &arg, sizeof(int));
  arg = 9;
  setEvent((struct timeval){1,0}, testEvent, &arg, sizeof(int));
  arg = 10;
  setEvent((struct timeval){2,0}, testEvent, &arg, sizeof(int));
  
  eventTimerQuit();
}
*/






