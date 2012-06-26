#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int main(int argc,char *argv[])
{
	int random_number,n,i;
	char *buffer,file_name[]="unsorted.txt";
	FILE *fp;

	srand ((unsigned)time(NULL));
	buffer = (char*)malloc(sizeof(int));

	n = atoi(argv[1]);
	fp = fopen(file_name,"w");

	for(i=0;i<n;i++)
	{
		random_number = (1 + rand() % 1);
		sprintf(buffer,"%d",random_number);
		strcat(buffer,"\n");
		fwrite(buffer,1,strlen(buffer),fp);
	}

	fclose(fp);

	return 0;
}
