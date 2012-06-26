#include <stdio.h>
#include <pthread.h>

#define MAX_SEQUENCE 10

typedef struct {
	long fib_sequence[MAX_SEQUENCE];
	int sequence_size;
}shared_data;

shared_data gfib_series;

void *threadFunction(void *);
void computeFibonacci(shared_data *);
void printFibonacci(shared_data *);

int main(int argc, char *argv[])
{
	int rc;
	pthread_t thread_id;
	long int long_int;
	char *end_ptr;

	if(argc != 2)
	{
		printf("\nInvalid input.\nFormat is: a.out <sequence size>\n\n");
		return -1;
	}

	long_int = strtol(argv[1],&end_ptr,10);
	if((!((atoi(argv[1]) >= 0) && (atoi(argv[1]) <= (MAX_SEQUENCE - 1)))) || (end_ptr == argv[1]) || (*end_ptr != '\0'))
	{
		printf("\nPlease enter Fibonacci Sequence size between 0 - 9.\n\n");
		return -1;
	}

	gfib_series.sequence_size = atoi(argv[1]);

	rc = pthread_create(&thread_id,NULL,threadFunction,argv[1]);
	if(rc != 0)
	{
		/* Error during pthread_create. */
		fprintf(stderr, "pthread_create failed.");
		return -1;
	}

	/* Wait for the thread to complete. */
	rc = pthread_join(thread_id,NULL);
	if(rc != 0)
	{
		/* Error during pthread wait creation. */
		fprintf(stderr, "pthread_join failed.");
		return -1;
	}

	printf("\nOuput from parent process:");
	printFibonacci(&gfib_series);

	return 0;
}

void *threadFunction(void *param)
{
	computeFibonacci(&gfib_series);

	printf("\nOuput from thread:");
	printFibonacci(&gfib_series);

	pthread_exit(0);
}

void computeFibonacci(shared_data *fib_series)
{
	int i;

	if(fib_series->sequence_size == 1)
	{
		fib_series->fib_sequence[0] = 0;
	}
	else if(fib_series->sequence_size == 2)
	{
		fib_series->fib_sequence[0] = 0;
		fib_series->fib_sequence[1] = 1;
	}
	else if(fib_series->sequence_size > 2)
	{
		fib_series->fib_sequence[0] = 0;
		fib_series->fib_sequence[1] = 1;

		for(i=2;i<fib_series->sequence_size;i++)
		{
			fib_series->fib_sequence[i] = fib_series->fib_sequence[i-1] + fib_series->fib_sequence[i-2];
		}
	}
}

void printFibonacci(shared_data *fib_series)
{
	int i;

	if(fib_series->sequence_size == 0)
	{
		printf("\nSize is 0. So Fibonacci series is empty.");
	}
	else
	{
		printf("\nFibonacci series is:");
		for(i=0;i<fib_series->sequence_size;i++)
		{
			printf("\n%ld",fib_series->fib_sequence[i]);
		}
	}
	printf("\n");
}
