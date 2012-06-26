#include<stdio.h>

#define MAX_LINE_LENGTH 100

int get_line(char [],int);

int main(int argc, char *argv[])
{
	char line[MAX_LINE_LENGTH];
	int len=0;

	while((len=get_line(line,MAX_LINE_LENGTH))>0) {
		printf("%3d %s",len,line);
	}

	return 0;
}

int get_line(char s[],int limit)
{
	int i,j,c;
	for(i=0;i<limit-1 && (c=getchar())!=EOF && c!='\n';i++) {
/*		if(c=='\t') {
			for(j=0;j<4;j++,i++) {
				s[i]='t';
			}
		} else {*/
			s[i]=c;
	//	}
	}
	if(c=='\n') {
		s[i]='\n';
		i++;
	}
	s[i]='\0';

	return (i);
}
