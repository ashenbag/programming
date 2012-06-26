#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

int main(int argc, char *argv[])
{
	struct addrinfo hints,*resolved, *p;
	struct sockaddr_in *ipv4;
	struct sockaddr_in6 *ipv6;
	int retvalue;
	void *addr;
	char *ipver;
	char ipstr[INET6_ADDRSTRLEN];

	printf("\n");

	if(argc != 2){
		printf("Usage: showip <hostname>\n");
		return -1;
	}

	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC; //IPV4 or IPV6
	hints.ai_socktype = SOCK_STREAM;

	if ((retvalue = getaddrinfo(argv[1],NULL,&hints,&resolved)) != 0) {
		fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(retvalue));

		return -1;
	}

	printf("IP address for %s: \n\n",argv[1]);

	for(p=resolved;p!=NULL;p=p->ai_next) {
		if(p->ai_family==AF_INET) {
			ipver="IPV4";
			ipv4=(struct sockaddr_in *)p->ai_addr;
			addr=&(ipv4->sin_addr);
		} else {
			ipver="IPV6";
			ipv6=(struct sockaddr_in6 *)p->ai_addr;
			addr=&(ipv6->sin6_addr);
		}
		
		inet_ntop(p->ai_family,addr,ipstr,sizeof(ipstr));
		printf("%s: %s\n",ipver,ipstr);
	}

	printf("\n");

	return 0;
}
