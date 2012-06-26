#include<stdio.h>

#define MAX_LINE_LENGTH 100

int get_line(char [],int);
void reverse(char []);

int main(int argc, char *argv[])
{
	char line[MAX_LINE_LENGTH];
	int len=0;

	while((len=get_line(line,MAX_LINE_LENGTH))>0) {
		printf("%s\n",line);
		reverse(line);
	}

	return 0;
}

void reverse(char string_input[])
{
	int i,j=0;;
	char reversed[MAX_LINE_LENGTH];

	for(i=0;(string_input[i]!='\0');i++);

	for(j=0;i>0;i--,j++)
		reversed[j]=string_input[i-1];

	reversed[j]='\0';
	
	printf("%s\n",reversed);

	return;
}

int get_line(char s[],int limit)
{
	int i,c;
	for(i=0;i<limit-1 && (c=getchar())!=EOF && c!='\n';i++) {
		s[i]=c;
	}
	//if(c=='\n') {
	//	s[i]='\n';
	//	i++;
	//}
	s[i]='\0';

	return (i);
}
