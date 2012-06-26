#include<stdio.h>

double calcpower(double,int);

int main()
{
	double base=2;
	int power=5;

	printf("\n\n");

	printf("Result:%f",calcpower(base,power));

	printf("\n\n");

	return 0;
}

double calcpower(double base,int power)
{
	double result;

	if(base == 0) {
		return 0;
	} else if(power == 1) {
		return base;
	}
	result = calcpower(base,power/2);

	result = result * result;

	return (power&1) ? (result * base) : result;
}
