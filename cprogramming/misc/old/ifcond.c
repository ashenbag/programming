#include<stdio.h>

int func(char *);

int main()
{
	int i;
	char a[] = "\0";

	if((i=func(a)) > 0) {
		printf("\nReturned greater than 0: %d.",i);
		return 0;
	} else {
		printf("\nReturned less than 0: %d.",i);
		return 0;
	}
}

int func(char *a)
{
	int input;
	printf("\nEnter value:");
	scanf("%d",&input);

	printf("\n\n func string: %s",a);

	return input;
}
