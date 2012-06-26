#include<stdio.h>

int fact(int);

int main()
{
	int n = 5;
//	printf("\nEnter number:");
//	scanf("%d",&n);

	printf("\nFactorial:%d\n\n",fact(n));
	return 0;
}

int fact(int n)
{
	if(n==1)
	{
		return 1;
	}
	return (n*fact(n-1));
}
