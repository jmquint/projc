#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <err.h>
#include "echo.h"


void func(int sig)
{
	pid_t cpid;
	int cstatus;
	cpid=wait(&cstatus);
	printf("received signal : %d\npid : %d, terminated with status : %d\n",sig,cpid,cstatus);
}

int main(int argc, char** argv)
{
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
		printf("bind error : %s, error number : %d\n",strerror(errno),errno);
		exit(EXIT_FAILURE);
	}
	if (listen(sfd,5)!=0)
	{
		printf("listen error\n");
		exit(EXIT_FAILURE);
	}
	pid_t forkstatus; 
	printf("waiting for connection...\n");
	signal(SIGCHLD,func);
	while (1)
	{

		struct sockaddr_in cli_addr;
		unsigned int clilen = sizeof(cli_addr);
		if ((newsock=accept(sfd,(struct sockaddr *)&cli_addr,&clilen)) == -1)
		{
			printf("accept problem : %s, error number : %d\n",strerror(errno),errno);
			exit(EXIT_FAILURE);
		}
		else
		{
			forkstatus=fork();
			if ( forkstatus == 0 )
			{
				close(sfd);
				printf("new connection to %d\n",getpid());
				echo(newsock,newsock);
				close(newsock);
				printf("close connection %d\n",getpid());
				printf("waiting for more connections...\n");
				exit(0);
			}
			else if (forkstatus>0)
			{
				close(newsock);
				continue;
			}
			else
			{
				printf("fork error\n");
				exit(EXIT_FAILURE);
			}
		}
		close(sfd);
	}

}
