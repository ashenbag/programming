#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#ifndef PTHREAD_THREADS_MAX
#define PTHREAD_THREADS_MAX 1024
#endif

#include "eventTimer.h"

#define TIMECMP(a,b,cmp) (((a).tv_sec cmp (b).tv_sec) || (((a).tv_sec == (b).tv_sec) && ((a).tv_usec cmp (b).tv_usec)))

#ifdef DEBUG
#define PRINTDEBUG(x) x
#else
#define PRINTDEBUG(x) 
#endif

//extern int errno;

typedef struct EE {
	struct timeval time;
	void (*handler)(void *);
	void *args;
	int argLen;
	struct EE *next;
} eventEntryType;

typedef struct TEQ {
	pthread_t id;
	eventEntryType eventQ;  
} threadEventQType;

threadEventQType threadEventQ[PTHREAD_THREADS_MAX];
int numThreads = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t handlerId;
sigset_t mask;

int insertEventQ (eventEntryType *eventQ, struct timeval time, void (*handler)(void *), void *args, int argLen) {
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

	for (cur = eventQ, pos = 0; (cur->next != NULL) && (TIMECMP(cur->next->time, time, <)); cur = cur->next, pos++);

	new->next = cur->next;
	cur->next = new;

	return pos;
}

void resetTimer (eventEntryType *eventQ, struct timeval time) {
	struct itimerval value = {{0,0}, {0,0}};

	if (eventQ->next == NULL) {
		return;
	}

	assert(TIMECMP(time, eventQ->next->time, <));

	value.it_value.tv_sec = eventQ->next->time.tv_sec - time.tv_sec;
	value.it_value.tv_sec -= (eventQ->next->time.tv_usec < time.tv_usec) ? 1 : 0;
	value.it_value.tv_usec = (eventQ->next->time.tv_usec < time.tv_usec) ? (eventQ->next->time.tv_usec + 1000000 - time.tv_usec) : (eventQ->next->time.tv_usec - time.tv_usec);

	PRINTDEBUG(printf("EventTimer: setting timer for %ld sec, %ld usec.\n", value.it_value.tv_sec, value.it_value.tv_usec));

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
	sigset_t oldmask;
	eventEntryType *eventQ;
	int i;

	PRINTDEBUG(printf("EventTimer: handling SIGALRM.\n"));

	if (pthread_sigmask(SIG_BLOCK, &mask, &oldmask) < 0) {
		fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
		exit(0);    
	}

	if (pthread_mutex_lock(&mutex) < 0) {
		fprintf(stderr, "Error: %d in event timer: pthread_mutex_lock.\n", errno);
		exit(0);    
	}

	for (i=0; i<numThreads; i++) {
		if (threadEventQ[i].id == pthread_self()) {
			break;
		}
	}
	assert(i < numThreads);

	eventQ = &threadEventQ[i].eventQ;

	if (eventQ->next == NULL) {
		if (pthread_mutex_unlock(&mutex) < 0) {
			fprintf(stderr, "Error: %d in event timer: pthread_mutex_unlock.\n", errno);
			exit(0);    
		}

		if (pthread_sigmask(SIG_SETMASK, &oldmask, NULL) < 0) {
			fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
			exit(0);    
		};

		return;
	}

	if (gettimeofday(&time, 0) < 0) {
		fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
		exit(0);
	}

	while (!TIMECMP(eventQ->next->time, time, >)) {
		temp = eventQ->next;
		eventQ->next = temp->next;
		handler = temp->handler;
		args = temp->args;
		free(temp);

		if (pthread_mutex_unlock(&mutex) < 0) {
			fprintf(stderr, "Error: %d in event timer: pthread_mutex_unlock.\n", errno);
			exit(0);    
		}

		if (pthread_sigmask(SIG_SETMASK, &oldmask, NULL) < 0) {
			fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
			exit(0);    
		};

		handler(args);

		if (args != NULL) {
			free(args);
		}

		if (eventQ->next == NULL) {
			return;
		}

		if (pthread_sigmask(SIG_BLOCK, &mask, &oldmask) < 0) {
			fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
			exit(0);    
		}

		if (pthread_mutex_lock(&mutex) < 0) {
			fprintf(stderr, "Error: %d in event timer: pthread_mutex_lock.\n", errno);
			exit(0);    
		}

		if (gettimeofday(&time, 0) < 0) {
			fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
			exit(0);
		}
	}

	resetTimer(eventQ, time);

	if (pthread_mutex_unlock(&mutex) < 0) {
		fprintf(stderr, "Error: %d in event timer: pthread_mutex_unlock.\n", errno);
		exit(0);    
	}

	if (pthread_sigmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
		exit(0);    
	};
}


int setEvent (struct timeval delay, void (*handler)(void *), void *args, int argLen) {
	struct timeval time, curTime;
	sigset_t oldmask;
	eventEntryType *eventQ;
	int i;

	if (handler == NULL || delay.tv_usec > 1000000 || delay.tv_sec < 0 || delay.tv_usec < 0 || (delay.tv_sec == 0 && delay.tv_usec == 0) || argLen < 0 || (argLen > 0 && args == NULL)) {
		errno = EINVAL;
		return -1;
	}

	if (pthread_sigmask(SIG_BLOCK, &mask, &oldmask) < 0) {
		fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
		exit(0);    
	}
	if (pthread_mutex_lock(&mutex) < 0) {
		fprintf(stderr, "Error: %d in event timer: pthread_mutex_lock.\n", errno);
		exit(0);    
	}

	for (i=0; i<numThreads; i++) {
		if (threadEventQ[i].id == pthread_self()) {
			break;
		}
	}

	if (i == numThreads) {
		if (numThreads == PTHREAD_THREADS_MAX) {
			fprintf(stderr, "Error: in event timer: exceeded max threads\n");
			exit(0);
		}
		threadEventQ[numThreads].id = pthread_self();
		threadEventQ[numThreads].eventQ = (eventEntryType){{0,0}, NULL, NULL};
		numThreads++;
	}

	eventQ = &threadEventQ[i].eventQ;

	if (gettimeofday(&curTime, 0) < 0) {
		fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
		exit(0);
	}

	time.tv_sec = curTime.tv_sec + delay.tv_sec + (curTime.tv_usec + delay.tv_usec)/(long)1000000;
	time.tv_usec = (curTime.tv_usec + delay.tv_usec)%(long)1000000;

	if (insertEventQ(eventQ, time, handler, args, argLen) == 0) {
		resetTimer(eventQ, curTime);
	}

	assert(eventQ->next != NULL);
	assert(threadEventQ[i].eventQ.next != NULL);

	if (pthread_mutex_unlock(&mutex) < 0) {
		fprintf(stderr, "Error: %d in event timer: pthread_mutex_unlock.\n", errno);
		exit(0);    
	}

	if (pthread_sigmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
		exit(0);    
	};

	return 0;
}

void eventTimerInit () {
	sigemptyset(&mask);
	sigaddset(&mask, SIGALRM);
	sigaddset(&mask, SIGIO);

	signal(SIGALRM, handleSigAlrm);
}

void eventTimerQuit () {
	eventEntryType *cur;
	struct timeval time;
	struct timeval delay = {0, 0};
	sigset_t oldmask;
	eventEntryType *eventQ;
	int i;

	if (pthread_sigmask(SIG_BLOCK, &mask, &oldmask) < 0) {
		fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
		exit(0);    
	};

	if (pthread_mutex_lock(&mutex) < 0) {
		fprintf(stderr, "Error: %d in event timer: pthread_mutex_lock.\n", errno);
		exit(0);    
	}

	for (i=0; i<numThreads; i++) {
		struct timeval tempDelay = {0, 0};
		eventQ = &threadEventQ[i].eventQ;

		if (eventQ->next != NULL) {
			for (cur = eventQ->next; cur->next != NULL; cur = cur->next);

			if (gettimeofday(&time, 0) < 0) {
				fprintf(stderr, "Error: %d in event timer: gettimeofday.\n", errno);
				exit(0);
			}

			tempDelay.tv_sec = (time.tv_sec > cur->time.tv_sec) ? 1 : (cur->time.tv_sec - time.tv_sec + 3);
			tempDelay.tv_usec = 0;
		}
		if (TIMECMP(tempDelay, delay, >)) {
			delay.tv_sec = tempDelay.tv_sec;
			delay.tv_usec = tempDelay.tv_usec;
		}
	}

	if (pthread_mutex_unlock(&mutex) < 0) {
		fprintf(stderr, "Error: %d in event timer: pthread_mutex_unlock.\n", errno);
		exit(0);    
	}

	if (pthread_sigmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		fprintf(stderr, "Error: %d in event timer: sigmask.\n", errno);
		exit(0);    
	};

	PRINTDEBUG(printf("EventTimer: waiting in thread %d for (%ld, %ld) time to quit.\n", threadEventQ[i].id, delay.tv_sec, delay.tv_usec));

	while (select(FD_SETSIZE, NULL, NULL, NULL, &delay) < 0) {
		if (errno != EINTR) {
			fprintf(stderr, "Error: %d in event timer: select.\n", errno);
			exit(0);
		}
	}

	PRINTDEBUG(printf("EventTimer: quitting.\n"));
}
