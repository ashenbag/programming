#include <stdio.h>

void fahr_to_celcius(float,float,float);

int main(int argc, char *argv[])
{
	float start,stop,step;
	
	start=0;
	stop=300;
	step=20;

	fahr_to_celcius(start,stop,step);
	
	return 0;
}
void fahr_to_celcius(float start,float stop,float step)
{
	float fahr;

	fahr=start;

	while(fahr<=stop) {
		printf("%3.0f %6.2f\n",fahr,((5.0/9.0)*(fahr-32)));
		fahr=fahr+step;
	}

	return;
}
