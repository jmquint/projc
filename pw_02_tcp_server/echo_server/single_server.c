#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <err.h>
#include "echo.h"

int main(int argc, char** argv)
{
	int c=0;
	ssize_t nread;
	char buf[1024];
	int errno=1;	
	if (argc != 2)
		errx(EXIT_FAILURE, "Usage:\n"
				"Arg 1 = Port number (e.g. 2048)");

	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd,newsock;
	int val=1;
	if (argc!=2)
	{
		printf("usage : %s port\n",argv[0]);
		exit(EXIT_FAILURE);
	}


	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;    /* IPv4 */
	hints.ai_socktype = SOCK_STREAM; /* TCP */
	hints.ai_flags = AI_PASSIVE; /* server mode */
	if (getaddrinfo(NULL, argv[1], &hints, &result)!=0)
	{
		printf("erreur getaddrinfo\n");
		exit(EXIT_FAILURE);
	}
	for (rp = result; rp != NULL; rp = rp->ai_next) 
	{
		c+=1;
		printf("compteur du for = %d\n",c);
		printf("flags: 0x%x\tfamily: %d\tsocktype: %d\tprotocol: %d\n",
				rp->ai_flags,
				rp->ai_family,
				rp->ai_socktype,
				rp->ai_protocol);
		if ((sfd = socket(rp->ai_family, rp->ai_socktype,0)) == -1)
		{
			printf("error opening a socket\n");
			continue;
		}
		if (setsockopt(sfd, SOL_SOCKET,SO_REUSEADDR,&val,sizeof(int)) == -1)
		{
			printf("setsockopt error\n");
			exit(EXIT_FAILURE);
		}

		if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(sfd);
	}
	freeaddrinfo(result);
	if (rp == NULL) 
	{
		printf("bid error : %s, error number : %d\n",strerror(errno),errno);
		exit(EXIT_FAILURE);
	}
	if (listen(sfd,5)!=0)
	{
		printf("listen error\n");
		exit(EXIT_FAILURE);
	}
	printf("waiting for connection...\n");
	while (1)
	{
		//printf("...\n");
		struct sockaddr_in cli_addr;
		unsigned int clilen = sizeof(cli_addr);
		if ((newsock=accept(sfd,(struct sockaddr *)&cli_addr,&clilen)) == -1)
		{
			printf("accept problem : %s, error number : %d\n",strerror(errno),errno);
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("connected\n");
			echo(newsock,STDOUT_FILENO);
			if (nread == -1) {
				printf("Read error");
				exit(EXIT_FAILURE);
			}


		}
	}
	//echo(STDIN_FILENO, STDOUT_FILENO);
	close(sfd);
}


