#include<stdio.h>

int match_int(int,int);

int main()
{
	int a,b,rc;
	int (*match)(int,int);
	printf("\nEnter number 1:");
	scanf("%d",&a);
	printf("\nEnter number 2:");
	scanf("%d",&b);

	match = &match_int;

	rc = (*match)(a,b);
	if(rc == 0)
	{
		printf("\nBoth are same.\n\n");
	}
	else
	{
		printf("\nBoth are not same.\n\n");
	}
	return 0;
}

int match_int(int a,int b)
{
	if(a==b)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
