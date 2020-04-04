#include <stdio.h>
#include <stdlib.h>

#include "print_page.h"

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

int main(int argc, char *argv[])
{
	printf("argc: %d\n",argc);
	printf("argv[0]: %s\n",argv[0]);
	printf("argv[1]: %s\n",argv[1]);
	if (argc!=2)
        	{
		printf("This program requires one parameter only\n");
		printf("   Usage : ./main <hostname>\n");
		exit(1);
		}
	//size_t l=0;
	print_page(argv[1]);
	printf("tf ");
}

