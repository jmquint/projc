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

int main()
{
	size_t l=0;
	const char h[]="example.com";
	print_page(h);
	printf("tf ");
}

