#include <stdio.h>

int main(int argc, char *argv[])
{
	float fahr;
	float start,stop,step;
	
	start=0;
	stop=300;
	step=20;

	fahr=start;

	while(fahr<=stop) {
		printf("%3.0f %6.2f\n",fahr,((5.0/9.0)*(fahr-32)));
		fahr=fahr+step;
	}
	return 0;
}
