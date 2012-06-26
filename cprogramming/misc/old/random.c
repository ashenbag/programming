#include<stdio.h>
#include<time.h>
#include<unistd.h>

int main()
{
	int random_number,i;

	srand ((unsigned)time(NULL));

	printf("\n\n\n");

	for(i=0;i<100;i++)
	{
		random_number = (1 + rand() % 5);
		sleep(1);
		printf("%d\n",random_number);
	}
	
	printf("\n\n\n");

	return 0;
}
