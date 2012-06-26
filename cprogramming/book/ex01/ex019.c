#include<stdio.h>
#define 
int main(int argc,char *argv[])
{
	int c;
	int comment_flag=0,start_flag=0,remove_flag=1;

	while((c=getchar())!=EOF) {
		if(c=='"' && start_flag ==0) {
			start_flag = 1;
			putchar(c);
			continue;
		} else if(c=='"' && start_flag == 1) {
			start_flag = 0;
			putchar(c);
			continue;
		} else if(c != '"' && c == '/' && comment_flag==0) {
			comment_flag=1;
			continue;
		} else if(c != '"' && c != '/' && comment_flag==1 && remove_flag==0) {
			comment_flag=0;
			remove_flag=0;
			putchar('/');
			putchar(c);
		} else if(c != '"' && c == '/' && comment_flag==1 && remove_flag==1) {
			comment_flag=0;
			continue;
		}
		putchar(c);
	}
	return 0;
}
