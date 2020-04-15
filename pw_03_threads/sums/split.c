#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>


#define INITIALIZE_ARRAY
struct thread_data
{
	long id;                    // Thread ID.
	pthread_t sys_id;           // Thread system ID.
	unsigned char *start;       // Points to the first element to add.
	long size;                  // Number of elements to add.
	unsigned long sum;          // Sum of the elements.
};

// Return the sum of an array of bytes.
// 'start' points to the first element of an array.
// 'size' is the number of elements of the array.
unsigned long linear_sum(unsigned char *start, long size)
{
	printf("entering linear_sum\n");
	unsigned long result=0;
	int i;
	for (i=0;i<size;i++)
	{
		printf("pt1\n");
		printf("result = %ld\n",result);
		printf("start = %s\n",start);
		result=result + *(start+i);
		printf("pt2\n");
		start++;
		printf("pt3\n");
	}
	printf("closing linear_sum\n");
	return result;

}

// Define the thread function.
void * worker(void *arg)
{
	// - Get the thread data passed as parameters.
	// - Call linear_sum().
	// - Store the result in the 'sum' field.
	// - Print the thread ID and the result.
	// - Return from the function.
	printf("entering worker\n");
	struct thread_data *thdata=arg;
	//thdata->sum=linear_sum(thdata->start,thdata->size);
	printf("thread id = %ld, result = %ld\n",thdata->id,thdata->sum);
	printf("closing worker\n");
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	// Get the arguments.
	// argv[1] = array_size = the size of the array of bytes (greater than 63).
	// argv[2] = thread_number = the number of threads (between 1 and 16).
	// -----------------------------------------------------------------------

	if (argc != 3)
		errx(EXIT_FAILURE, "Usage: array_size thread_number");

	long array_size = atol(argv[1]);
	if (array_size <= 63)
		errx(EXIT_FAILURE, "The size of the array is not valid (must be greater than 63).");

	long thread_number = atol(argv[2]);
	if (thread_number < 1 || thread_number > 16)
		errx(EXIT_FAILURE, "The number of threads is not valid (between 1 and 16).");

	// -----------------------------------------------------------------------

	// Data for the threads.
	struct thread_data data[thread_number];

	// Allocate the array of bytes.
	unsigned char *bytes = malloc(array_size);
	if (bytes == NULL)
		errx(EXIT_FAILURE, "Not enough memory!\nTry to reduce the size of the array.");

#ifdef INITIALIZE_ARRAY
	// Initialize the array.
	printf("Initializing array........");
	fflush(stdout);
	for (long i = 0; i < array_size; i++)
		bytes[i] = 1;
	printf("OK\n");
#endif

	// - Determine the size of a chunk.
	//   Be careful, the size of the last chunk may include some remaining bytes.
	// - For each chunk:
	//     - Set the thread_data structure of the thread.
	//       This structure is stored in the 'data' array.
	//       (The thread ID should be used as index.)
	//     - Execute the thread.
	// Wait for the threads and add up their sums.
	// Print the sum of the array.
	// Free the array.
	// Return from the function.
	long chunk_size=(array_size/thread_number);
	long last_chunk_size=chunk_size+(array_size-(thread_number*chunk_size));
	long i;
	//unsigned long *first_sum=0;
	pthread_t thread[thread_number];
	for (i=0;i<(thread_number-1);i++)
	{
		printf("i = %ld\n",i);
		data[i].id=i;
		data[i].start=(unsigned char *)(i*chunk_size);
		data[i].size=chunk_size;
		//data[i]->sum=first_sum;
		printf("one thread created\n");	
		printf("d = %d, p = %p, s = %s\n",data[i].start,data[i].start,data[i].start);

		pthread_create(&thread[i],NULL,worker,data);
	}
	i=thread_number-1;
	printf("i = %ld\n",i);
	data[i].id=i;
	data[i].start=(unsigned char *)(i*chunk_size);
	data[i].size=last_chunk_size;
	//data[i]->sum=first_sum;
	printf("creating last thread\n");
	printf("d = %d, p = %p, s = %s\n",data[i].start,data[i].start,data[i].start);
	pthread_create(&thread[i],NULL,worker,data);


	long result=0;
	for (i=0;i<(thread_number);i++)
	{
		printf("td\n");
		if (pthread_join(thread[i],NULL)!=0)
		{
			printf("error joining\n");
			exit(1);
		}
		printf("tm\n");
		result+=data[i].sum;
		printf("tf\n");
	}

	printf("TOTAL SUM = %ld\n",result);
	return 0;


}
