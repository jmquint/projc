#define _GNU_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>

#include <errno.h>


char *build_query(const char *host, size_t *len);
void print_page(const char *host);

char *build_query(const char *host, size_t *len)
{
	char *p;
	*len=asprintf(&p,"GET http://www.%s HTTP/1.0\r\n\r\n",host);
	if (*len==-1)
		exit(1);
	return p;
}

void print_page(const char *host)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd;
	const char port[]="80";
	ssize_t nread;
	char buf[8192];
	size_t lenHost;
	char *query=build_query(host,&lenHost);
	errno=0;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	if (getaddrinfo(host, port, &hints, &result)!=0)
	{

		printf("problem getaddrinfo, host not found: %d\n",getaddrinfo(host, port, &hints, &result));	//error getaddrinfo
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) 
	{
		if (hints.ai_family!=AF_INET || hints.ai_socktype!=SOCK_STREAM)
		{
			printf("Host not compatible");
			exit(1);
		}
		sfd = socket(rp->ai_family,rp->ai_socktype,0);
		if (sfd == -1)
		{
			printf("socket problem: %s, error number : %d\n",strerror(errno),errno);	//error with socket
			continue;
		}

		if (connect(sfd, result->ai_addr, rp->ai_addrlen) != -1)
		{
			printf("Connected\n");	// connected
			break;                 
		}
		else
		{
			printf("connection problem: %s, error number : %d\n",strerror(errno),errno);	//error with connection
			exit(EXIT_FAILURE);
		}

		close(sfd);

	}
	
	freeaddrinfo(result);

	if (write(sfd, query, lenHost) == -1) {
		printf("Write error");
		exit(EXIT_FAILURE);
	}
	nread = read(sfd, buf, 8192);
	if (nread == -1) {
		printf("Read error");
		exit(EXIT_FAILURE);
	}
	printf("Received %zd bytes: %s\n", nread, buf);
}

