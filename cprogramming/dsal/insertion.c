#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void insertion_sort(int *,int);

int main(int argc,char *argv[])
{
	int input_size,*input_numbers,*input,i,random_number;

	input_size = atoi(argv[1]);
	input_numbers = (int*)malloc(sizeof(int)*input_size);
	input = input_numbers;

	srand ((unsigned)time(NULL));

	for(i=0;i<input_size;i++)
	{
		random_number = (1 + rand() % 100);
		*input_numbers = random_number;
		input_numbers++;
	}
	input_numbers = input;

	printf("\nBefore Sorting...\n");
	for(i=0;i<input_size;i++)
	{
		printf("%d\n",*input_numbers);
		input_numbers++;
	}
	input_numbers = input;

	insertion_sort(input_numbers,input_size);

	printf("\nAfter Sorting...\n");
	for(i=0;i<input_size;i++)
	{
		printf("%d\n",*input_numbers);
		input_numbers++;
	}
	input_numbers = input;

	return 0;
}

void insertion_sort(int *input_numbers,int input_size)
{
	int i,j,key;

	for(i=1;i<input_size;i++)
	{
		key = input_numbers[i];
		j=i-1;
		while(j>=0 && input_numbers[j]>key)
		{
			input_numbers[j+1]=input_numbers[j];
			j--;
		}
		input_numbers[j+1]=key;
	}
	return;
}
