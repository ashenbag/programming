#include<stdio.h>

#define MAX_LINE_LENGTH 100
#define LIMIT 5

int get_line(char [],int);

int main(int argc, char *argv[])
{
	char line[MAX_LINE_LENGTH];
	int len;

	while((len=get_line(line,MAX_LINE_LENGTH))>0) {
		printf("%d %s",len,line);
	}

	return 0;
}

int get_line(char s[],int limit)
{
	int i,c;
	int state=0;

	for(i=0;i<limit-1 && (c=getchar())!=EOF && c!='\n';i++) {
		s[i]=c;
		if(c == ' ') {
			state = 0;
			space_count++;
		} else if(c =='t') {
			state = 0;
			tab_count++;
		} else if(c != '\n') {
			state=1;
		}
	}
	if(state == 0) {

	}
	if(c=='\n') {
		s[i]='\n';
		i++;
	}
	s[i]='\0';

	return (i);
}
