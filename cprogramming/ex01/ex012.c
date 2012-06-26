#include<stdio.h>

#define MAX_WORD_LENGTH 10

int main(int argc, char *argv[])
{
	int c,length=0,i,j;
	int STATE=0;
	int histogram[MAX_WORD_LENGTH+1];

	for(i=0;i<=MAX_WORD_LENGTH;i++) {
		histogram[i] = 0;
	}

	while((c=getchar())!=EOF) {
		if(c == ' ' || c == '\n' || c == '\t') {
			if(STATE==1) {
				histogram[length]++;
			}
			STATE=0;
			length=0;
			
		} else {
			STATE=1;
			length++;
		}
	}

	for(i=1;i<=MAX_WORD_LENGTH;i++) {
		printf("%d\t\t",i);
		for(j=0;j<histogram[i];j++) {
			printf("*");
		}
		printf("\n");
	}

	return 0;
}
