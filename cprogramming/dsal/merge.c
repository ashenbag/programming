#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

/* Since random numbers are less than 100, 10000 is a big number! */
#define BIGNUMBER 10000

void merge_sort(int *,int,int);
void merge(int *,int,int,int);

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

	merge_sort(input_numbers,0,input_size-1);

	printf("\nAfter Sorting...\n");
	for(i=0;i<input_size;i++)
	{
		printf("%d\n",*input_numbers);
		input_numbers++;
	}
	input_numbers = input;

	return 0;
}

void merge_sort(int *input_numbers,int p,int r)
{
	int q;
	if(p<r)
	{
		q = floor((p+r)/2);
		merge_sort(input_numbers,p,q);
		merge_sort(input_numbers,q+1,r);
		merge(input_numbers,p,q,r);
	}
	return;
}

void merge(int *input_numbers,int p,int q,int r)
{
	int n1,n2,*left,*right,i,j,k;
	
	n1=q-p+1;
	n2=r-q;
	left=(int*)malloc(sizeof(int)*(n1+1));
	right=(int*)malloc(sizeof(int)*(n2+1));

	printf("\n p:%d q:%d r:%d",p,q,r);

	for(i=0;i<n1;i++)
	{
		left[i]=input_numbers[p+i];
		printf("\nleft%d",left[i]);

	}
	for(j=0;j<n2;j++)
	{
		right[j]=input_numbers[q+j+1];
		printf("\nright%d",right[j]);
	}

	left[n1+1]=BIGNUMBER;
	right[n2+1]=BIGNUMBER;

	i=0;
	j=0;

	for(k=p;k<=r;k++)
	{
		if(left[i]<=right[j])
		{
			input_numbers[k]=left[i];
			i++;
		}
		else
		{
			input_numbers[k]=right[j];
			j++;
		}
	}

	return;
}
