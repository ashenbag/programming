#include<stdio.h>

#define MAX_LINE_LENGTH 100

int get_line(char [],int);
void copy_line(char [],char []);

int main(int argc, char *argv[])
{
	char line[MAX_LINE_LENGTH];
	char longest[MAX_LINE_LENGTH];
	int len=0,max=0;

	while((len=get_line(line,MAX_LINE_LENGTH))>0) {
		if(len > max) {
			max=len;
			copy_line(line,longest);
		}
	}
	if(max>0) {
		printf("Longest line length is %d and line is %s",max,longest);
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

void copy_line(char source[],char destination[])
{
	int i=0;

	while((destination[i]=source[i])!='\0') {
		i++;
	}
}
