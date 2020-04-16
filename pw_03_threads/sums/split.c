#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>


//#define INITIALIZE_ARRAY
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
	unsigned long result=0;
	int i;
	for (i=0;i<size;i++)
	{
		result=result + 1;
		start++;
	}
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
	struct thread_data *thdata=arg;
	printf("Thread %02ld: %ld\n",thdata->id,thdata->size);
	thdata->sum=linear_sum(thdata->start,thdata->size);
	pthread_exit((void *)thdata->sum);
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
	
	
	for (i=0;i<thread_number;i++)
	{
		data[i].id=i;
		if (i==thread_number-1)
		{
			data[i].size=last_chunk_size;
		}
		else
		{
			data[i].size=chunk_size;
		}
		data[i].start=malloc(data[i].size*sizeof(long));

		pthread_create(&thread[i],NULL,worker,(void *)&data[i]);
	}
	void *final_sum;
	
	long result=0;
	for (i=0;i<(thread_number);i++)
	{
		if (pthread_join(thread[i],&final_sum)!=0)
		{
			printf("error joining\n");
			exit(1);
		}
		result=result+(unsigned long)final_sum;
	}

	printf("Sum......: %ld\n",result);
	return 0;


}
