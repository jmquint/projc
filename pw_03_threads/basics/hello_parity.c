#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>



void* fn_thread(void* arg);

int main(int argc, char** argv)
{
	// Check the number of arguments.
	if (argc < 2)
		errx(EXIT_FAILURE, "The number of threads is missing.");
	else if (argc > 2)
		errx(EXIT_FAILURE, "Specify only the number of threads.");

	// - Convert the argument into a long integer.
	//   Use atol(3).
	//   This value represents the number of threads.
	// - If the argument is not valid (i.e. lower than or equal to zero),
	//   exit with an error message.
	long number=atol(argv[1]);
	if (number<=0)
	{
		printf("error argument\n");
		exit(1);
	}
	pthread_t thread[number];



	// - Create and execute the threads.
	//   If an error occurs, exit with an error message.
	//   You can use err(3), but the 'errno' variable is not set automatically.
	//   You have to set it manually to the return value of pthread_create().
	long i;
	char s[2][64]={"Hello from an even thread.","Hi from an odd thread."};
	for (i=0;i<number;i++)
	{

		if (pthread_create(&thread[i],NULL,fn_thread,s[i%2])!=0)
		{
			printf("error creating\n");
			exit(1);
		}
	}
	// - Wait for the threads to end.
	for (i=0;i<number;i++)
	{
		if (pthread_join(thread[i],NULL)!=0)
		{
			printf("error joining\n");
			exit(1);
		}
	}

	// - Return from the function.
	return 0;
}

// Define the thread function.
void* fn_thread(void* arg)
{
	// - Wait to simulate execution time
	sleep(rand()%3);

	// - Cast arg to long
	char *str=arg;
	// - Print a message.
	printf("%s\n",str);

	// - Return from the function.
	pthread_exit(NULL);
}
