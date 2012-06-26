#include<stdio.h>

int fact(int);

int main()
{
	int n;
	printf("\nEnter number:");
	scanf("%d",&n);

	printf("\nFactorial:%d\n\n",fact(n));
	return 0;
}

int fact(int n)
{
	int i=0,a;
/*	
	a = n;
	for(i=1;i<n;i++)
	{
		a=a*(n-i);
	}
*/
	a=1;
	for(i=1;i<=n;i++)
	{
		a=a*i;
	}
	return a;
}
