#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 5
#define FACTOR 0.000000001

/* These constraints can be changed based on the need. */
#define MAX_PRODUCERS 10
#define MAX_CONSUMERS 10
#define MAX_SLEEPTIME 60

#define TRUE 1

int buffer[BUFFER_SIZE],in=0,out=0;
pthread_mutex_t mutex;
sem_t empty,full;

void *producer(void *);
void *consumer(void *);
void insert_item(int);
void remove_item(int *);

int main(int argc, char *argv[])
{
	int rc,i;
	int sleep_time,num_prod,num_cons,prod_id[MAX_PRODUCERS],cons_id[MAX_CONSUMERS];
	pthread_t prod_thread_id[MAX_PRODUCERS],cons_thread_id[MAX_CONSUMERS];
	char *end_ptr;

	/* Validating input */
	if(argc != 4)
	{
		printf("\nInvalid input.\nFormat is: ./a.out <sleep time> <number of producers> <number of consumers> \n\n");
		return -1;
	}
	strtol(argv[1],&end_ptr,10);
	if(!((atoi(argv[1]) >= 1) && (atoi(argv[1]) <= MAX_SLEEPTIME)) || (end_ptr == argv[1]) || (*end_ptr != '\0'))
	{
		printf("\nPlease enter number of sleep time from 1 to 60.\n\n");
		return -1;
	}
	strtol(argv[2],&end_ptr,10);
	if(!((atoi(argv[2]) >= 1) && (atoi(argv[2]) <= MAX_PRODUCERS)) || (end_ptr == argv[1]) || (*end_ptr != '\0'))
	{
		printf("\nPlease enter number of producers from 1 to 10.\n\n");
		return -1;
	}
	strtol(argv[3],&end_ptr,10);
	if(!((atoi(argv[3]) >= 1) && (atoi(argv[3]) <= MAX_CONSUMERS)) || (end_ptr == argv[1]) || (*end_ptr != '\0'))
	{
		printf("\nPlease enter number of consumers from 1 to 10.\n\n");
		return -1;
	}

	/* Assiging input value to variables */
	sleep_time = atoi(argv[1]);
	num_prod = atoi(argv[2]);
	num_cons = atoi(argv[3]);

	/* Initializing */	
	memset(buffer,0,(sizeof(int)*BUFFER_SIZE));	
	pthread_mutex_init(&mutex,NULL);
	sem_init(&empty,0,BUFFER_SIZE);
	/* Counting semaphare empty initialized to BUFFER_SIZE indicating buffers have no data at the start of program.
	 * This prevents customers from accessing buffer when no data is present and informing producers that buffer is free. */
	sem_init(&full,0,0);


	/* Creating Producer threads */
	for(i=1;i<=num_prod;i++)
	{
		prod_id[i] = i;
		rc = pthread_create(&prod_thread_id[i],NULL,producer,&prod_id[i]);
		if(rc != 0)
		{
			/* Error during pthread_create. */
			fprintf(stderr, "producer pthread_create failed.");
			return -1;
		}
	}

	/* Creating consumer threads */
	for(i=1;i<=num_cons;i++)
	{
		cons_id[i] = i;
		rc = pthread_create(&cons_thread_id[i],NULL,consumer,&cons_id[i]);
		if(rc != 0)
		{
			/* Error during pthread_create. */
			fprintf(stderr, "consumer pthread_create failed.");
			return -1;
		}
	}

	/* Producer and Consumer threads execute as long as main sleeps for sleep_time */
	sleep(sleep_time);

	return 0;
}
void *producer(void *param)
{
	int item,*prod_id;
	prod_id = (int *)param;
	while(TRUE)
	{
		sleep((rand() * FACTOR));

		item = rand();

		sem_wait(&empty);
		insert_item(item);
		printf("Producer %d produced %d\n",(*prod_id),item);
		sem_post(&full);
	}
}
void *consumer(void *param)
{
	int item,*cons_id;
	cons_id = (int *)param;
	while(TRUE)
	{
		sleep((rand() * FACTOR));

		sem_wait(&full);
		remove_item(&item);
		printf("Consumer %d consumed %d\n",(*cons_id),item);
		sem_post(&empty);
	}
}
void insert_item(int item)
{
	pthread_mutex_lock(&mutex);
	buffer[in] = item;
	in = (in + 1) % BUFFER_SIZE;
	pthread_mutex_unlock(&mutex);
}
void remove_item(int *item)
{
	pthread_mutex_lock(&mutex);
	*item = buffer[out];
	out = (out + 1) % BUFFER_SIZE;
	pthread_mutex_unlock(&mutex);
}
