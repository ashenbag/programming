#include<stdio.h>
#include<math.h>
#include<time.h>

#define MVA_PERIOD 20000

int main()
{
	double input=0;
	double temp[MVA_PERIOD],sum=0,mva=0,data_count=1;
	//printf("\nInput:");
	
	srand ((unsigned)time(NULL));

	//while(input != -1)
	while(data_count < 1000000)
	{
		//scanf("%f",&input);
		input = (1 + rand() % 100)/100.0;
		printf("\nInput:%f",input);

		if(input == -1) {
			break;
		}
		data_count++;
		if(data_count-1 < MVA_PERIOD) {
			temp[(int)((data_count-1)) - 1] = input;
			sum = sum + input;
		} else if (data_count-1 == MVA_PERIOD) {
			temp[(MVA_PERIOD) - 1] = input;
			sum = sum + input;
			mva = sum/MVA_PERIOD;
			printf("\nMVA:%f",mva);
		} else if (data_count-1 > MVA_PERIOD) {
			if(((int)fmod((data_count-1),(MVA_PERIOD))) == 0) {
				sum = sum - temp[((int)fmod((data_count-1),(MVA_PERIOD))) + (MVA_PERIOD - 1)];
				temp[((int)fmod((data_count-1),(MVA_PERIOD))) + (MVA_PERIOD - 1)] = input;
			} else {
				sum = sum - temp[((int)fmod((data_count-1),(MVA_PERIOD))) - 1];
				temp[((int)fmod((data_count-1),(MVA_PERIOD))) - 1] = input;
			}
			sum = sum + input;
			mva = sum / MVA_PERIOD;
			printf("\nMVA:%f",mva);
		}
		//printf("\n");
	}
	return 0;
}
