#include<stdio.h>

int main(int argc, char *argv[])
{	
	int c;

	while((c=getchar())!=EOF) {
		if(c==' ') {
			putchar(c);
			while((c=getchar())==' '){
				;
			}
		}
		putchar(c);
	}

	return 0;
}
