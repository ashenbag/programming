#include<stdio.h>
#include<syslog.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int i=0;
	char syslog_message[1024]="",buf[4]="";

	openlog(argv[0],LOG_PID,LOG_LOCAL0);

	for(i=0;i<3000;i++) {
		if(i%10==0) {
			sprintf(buf,"%d",i);
			strcat(syslog_message,buf);
		} else {
			strcat(syslog_message,".");
		}
	}

	syslog(LOG_WARNING,"%s",syslog_message);

	closelog();
	
	return 0;
}
