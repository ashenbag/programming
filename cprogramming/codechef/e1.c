#include<stdio.h>

int main()
{
	int X;
	float Y;

	scanf("%d %f",&X,&Y); 

	if((X % 5) != 0) {
		printf("\n%0.2f",Y);
	} else {
		Y = Y -(X+0.5);
		if(Y<0) {
			printf("\n%0.2f",(Y+(X+0.5)));
		} else {
			printf("\n%0.2f",Y);
		}
	}	
	printf("\n");

	return 0;
}
