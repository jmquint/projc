#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

#define BUFFER_SIZE 512

void echo(int fd_in, int fd_out)
{
	char buf[BUFFER_SIZE];
	int r;
	while ((r=read(fd_in,buf,BUFFER_SIZE))>0)
	{
		if (write(fd_out,buf,r)==-1)
		{
			printf("write error");
			exit(EXIT_FAILURE);
		}
	}
}
