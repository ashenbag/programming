#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>

#define MAX_SEQUENCE 10

typedef struct {
	long fib_sequence[MAX_SEQUENCE];
	int sequence_size;
}shared_data;

void computeFibonacci(shared_data *);
void printFibonacci(shared_data *);

int main(int argc, char *argv[])
{
	int user_input,segment_id,rc;
	char *shared_memory;
	shared_data *fib_series;
	pid_t process_id;
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
		/* Integer check */
		printf("\nPlease enter Fibonacci Sequence size between 0 - 9.\n\n");
		return -1;
	}

	segment_id = shmget(IPC_PRIVATE,sizeof(shared_data),S_IRUSR | S_IWUSR);
	if(segment_id < 0)
	{
		/* Error during Shared memory Allocation (shmget) */
		fprintf(stderr, "shmget failed.");
		return -1;
	}

	shared_memory = shmat(segment_id,NULL,0);
	if(shared_memory == NULL)
	{
		/* Error during Shared memory Attach (shmat) */
		fprintf(stderr, "shmat failed.");
		return -1;
	}

	fib_series = (shared_data *)shared_memory;

	fib_series->sequence_size = atoi(argv[1]);

	process_id = fork();

	if(process_id < 0)
	{
		/* Error during child process creation. */
		fprintf(stderr, "Fork failed");
		return -1;
	}
	else if(process_id == 0)
	{
		/* Child process */

		computeFibonacci(fib_series);
		printf("\nOuput from child process:");
		printFibonacci(fib_series);
		rc = shmdt(shared_memory);
		if(rc == -1)
		{
			/* Error during Shared memory Detach in Child. */
			fprintf(stderr,"shmdt failed in child.");
			return -1;
		}
	}
	else
	{
		/* Parent process */

		/* Wait for the child to complete. */
		wait(NULL);
		printf("\nOuput from parent process:");
		printFibonacci(fib_series);
		rc = shmdt(shared_memory);
		if(rc == -1)
		{
			/* Error during Shared memory Detach in Child. */
			fprintf(stderr,"shmdt failed in parent.");
			return -1;
		}
		rc = shmctl(segment_id,IPC_RMID,NULL);
		if(rc == -1)
		{
			/* Error during Shared memory Delete */
			fprintf(stderr,"shmctl failed.");
			return -1;
		}
	}

	return 0;
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
