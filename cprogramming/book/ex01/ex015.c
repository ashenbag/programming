#include<stdio.h>

#define MAX_LINE_LENGTH 100
#define LIMIT 5

int get_line(char [],int);

int main(int argc, char *argv[])
{
	char line[MAX_LINE_LENGTH];
	int len=0;

	while((len=get_line(line,MAX_LINE_LENGTH))>0) {
		if(len > LIMIT) {
			printf("%dLine: %s",len,line);
			len=0;
		}
	}

	return 0;
}

int get_line(char s[],int limit)
{
	int i,c;
	for(i=0;i<limit-1 && (c=getchar())!=EOF && c!='\n';i++) {
		s[i]=c;
	}
	if(c=='\n') {
		s[i]='\n';
		i++;
	}
	s[i]='\0';

	return (i);
}
