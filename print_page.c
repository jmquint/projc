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

int main()
{
	const char text[]="salut";
	size_t l=666;
	//printf("test1");
	build_query(text,&l);
	const char h[]="example.com";
	printf("td ");
	print_page(h);
	printf("tf ");
}

char *build_query(const char *host, size_t *len)
{
	char *p;
	*len=asprintf(&p,"GET http://www.%s HTTP/1.0\r\n\r\n",host);
	if (*len==-1)
		exit(1);
	printf("%s\n",p);
	return p;
}

void print_page(const char *host)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;
	const char port[]="80";
	size_t len;
	ssize_t nread;
	char buf[1024];
	errno=0;
	printf("t1 ");
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family=AF_INET;
	hints.ai_socktype=SOCK_STREAM;
	printf("t2 ");
	if (getaddrinfo(host, port, &hints, &result)!=0)
	{
		printf("erreur getaddrinfo %d\n",getaddrinfo(host, port, &hints, &result));	//error getaddrinfo
		exit(EXIT_FAILURE);
	}
	printf("t3 ");
	for (rp = result; rp != NULL; rp = rp->ai_next) 
	{
		sfd = socket(rp->ai_family,rp->ai_socktype,0);
		if (sfd == -1)
		{
			printf("socket problem %s, error number : %d\n",strerror(errno),errno);	//error with socket
			continue;
		}

		if (connect(sfd, result->ai_addr, rp->ai_addrlen) != -1)
		{
			printf("Connected\n");	// connected
			break;                 
		}
		else
		{
			printf("Error connection");	//error with connection
			exit(EXIT_FAILURE);
		}
		close(sfd);
	}
	printf("t4 ");
	if (rp == NULL) {               // no working address found
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(result);

	if (write(sfd, host, strlen(host)+1) == -1) {
		printf("Write error");
		exit(EXIT_FAILURE);
	}
	nread = read(sfd, buf, 512);
	if (nread == -1) {
		printf("Read erro");
		exit(EXIT_FAILURE);
	}
	printf("Received %zd bytes: %s\n", nread, buf);
}
