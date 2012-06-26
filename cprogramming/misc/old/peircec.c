#include<stdio.h>
#include<math.h>

#define MAX_N 8
#define MAX_TO 15
#define MAX_DO 5

int data_print(void);
double data_mean(void);
double data_variance(double);
double get_pctable_value(int,int);
int pc_outlier(double,double);

double pctable[MAX_TO][MAX_DO] = {
	{0.000,0.000,0.000,0.000,0.000},
	{0.000,0.000,0.000,0.000,0.000},
	{1.196,0.000,0.000,0.000,0.000},
	{1.383,1.078,0.000,0.000,0.000},
	{1.509,1.200,0.000,0.000,0.000},
	{1.610,1.299,1.099,0.000,0.000},
	{1.693,1.382,1.187,1.022,0.000},
	{1.763,1.453,1.261,1.109,0.000},
	{1.824,1.515,1.324,1.178,1.045},
	{1.878,1.570,1.380,1.237,1.114},
	{1.925,1.619,1.430,1.289,1.172},
	{1.969,1.663,1.475,1.336,1.221},
	{2.007,1.704,1.516,1.379,1.266},
	{2.043,1.741,1.554,1.417,1.307},
	{2.076,1.775,1.589,1.453,1.344}};

struct data{
	double value;
	int outlier;
} a[] = {
	{0.999058,0},
	{0.993587,0},
	{1.67,0},
	{0.985579,0},
	{0.982124,0},
	{1.981647,0},
	{0.996694,0},
	{1.011349,0}
/*	{101.2,0},
	{90.0,0},
	{99.0,0},
	{102.0,0},
	{103.0,0},
	{100.2,0},
	{89.0,0},
	{98.1,0},
	{101.5,0},
	{102.0,0}*/
};

int main()
{
	int retValue;
	double mean,variance,stddev;

	mean = data_mean();
	printf("\nMean: %f",mean);

	variance = data_variance(mean);
	printf("\nVariance: %f",variance);

	stddev = sqrt(variance);
	printf("\nStandard Deviation: %f",stddev);

	retValue = pc_outlier(mean,stddev);

	printf("\n\n");

	return 0;
}

int data_print(void)
{
	int i;
	printf("\nPrinting data...");
	for(i=0;i<MAX_N;i++) {
		printf("\n%f",a[i].value);
	}
	return 0;
}

double data_mean(void)
{
	int i;
	double sum=0;
	for(i=0;i<MAX_N;i++) {
		sum=sum+a[i].value;
	}
	return (sum/MAX_N);
}

double data_variance(double mean)
{
	int i;
	double sum=0;

	for(i=0;i<MAX_N;i++) {
		sum=sum+pow((a[i].value - mean),2);
	}

	return (sum/(MAX_N-1));
}

double get_pctable_value(int total_obv,int doubt_obv)
{
	return pctable[(total_obv - 1)][(doubt_obv - 1)];
}

int pc_outlier(double mean,double stddev)
{
	int i,j,none_found;
	double r,maxdev;
	for(i=1;i<=MAX_N;i++) {
		r=get_pctable_value(MAX_N,i);
		none_found = 1;
		maxdev = stddev * r;
		for(j=0;j<MAX_N;j++) {
			if(a[j].outlier == 0) {
				if(fabs(a[j].value - mean) > maxdev) {
				//if((a[j].value - mean) > maxdev) {
					printf("\nOutlier: %f",a[j].value);
					a[j].outlier = 1;
					none_found = 0;
				}
			}
		}
		if(none_found == 1) {
			break;
		}
	}

	return 0;
}
