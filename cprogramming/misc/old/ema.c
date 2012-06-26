#include<stdio.h>

#define alpha 0.1

int main()
{
	int a[12] = {71,70,69,68,64,65,72,78,75,75,75,70},i;
	float s=a[0];

	for(i=1;i<12;i++)
	{
		printf("\n%d: %f",i,s);
		s = ((alpha * a[i]) + ((1-alpha)*s));
	}

	printf("\n");

	return 0;
}
