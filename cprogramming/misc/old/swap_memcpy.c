#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void swap(void *,void *,int);

int main()
{
	int a=10,b=20;

	printf("\nBefore swap: a:%d, b:%d\n\n",a,b);

	swap(&a,&b,sizeof(int));

	printf("\nAfter swap: a:%d, b:%d\n\n",a,b);

	return 0;
}

void swap(void *a,void *b,int size)
{
	void *temp;

	temp = (void*)malloc(size);

	memcpy(temp,a,size);
	memcpy(a,b,size);
	memcpy(b,temp,size);

	free(temp);

	return;
}
