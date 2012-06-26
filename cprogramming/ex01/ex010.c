#include<stdio.h>

int main(int argc, char *argv[])
{	
	int c;

	while((c=getchar())!=EOF) {
		if(c=='\t') {
			c='t';			
		}
		putchar(c);
	}

	return 0;
}
