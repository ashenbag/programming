#include<stdio.h>
#include<string.h>

#define MAX_LINE_SIZE 100

int main()
{
	char inputfile[]="files";
	char *filename;
	char path[MAX_LINE_SIZE];
	char *temp;
	char line[MAX_LINE_SIZE];

	FILE *gitfp;
	FILE *infp;

	infp = fopen(inputfile,"r");

	while(fgets(line,sizeof(line),infp)!=NULL) {
		fputs(line,stdout);
		temp = strrchr(line,'/');

		filename = temp+1;
		*temp = '\0';
		strcpy(path,line);

		printf("%s\n",path);
		printf("%s\n",filename);

		strcat(path,"/.gitignore");

		printf("%s\n",path);

		gitfp=fopen(path,"a");

		printf("%s\n",filename);
		fprintf(gitfp,"%s",filename);

		fclose(gitfp);
	}
	fclose(infp);

	printf("Done.\n");

	return 0;
}
