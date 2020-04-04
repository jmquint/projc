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
	if (argc!=2)
        	{
		printf("This program requires one parameter only\n");
		printf("   Usage : ./main <hostname>\n");
		exit(1);
		}
	print_page(argv[1]);
}

