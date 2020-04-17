#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <err.h>
#include <stdatomic.h>

#define INITIALIZE_ARRAY

void * worker(void *arg);

struct thread_data
{
	unsigned char *start;       // Points to the first element to add.
	long size;                  // Number of elements to add.
	long threshold;             // Threshold.
	unsigned long sum;          // Sum of the elements.
};

// Return the sum of an array of bytes.
// 'start' points to the first element of an array.
// 'size' is the number of elements of the array.
unsigned long linear_sum(unsigned char *start, long size)
{
	unsigned long result=0;
	long i;
	printf("LINEAR -- start p= %p, start d= %d\n",start,*start);
		printf("size= %ld\n",size);
	for (i=0;i<size;i++)
	{
		result=result + (long)(*start);
		start++;
	}
	return result;
}

unsigned long dnc_sum(unsigned char *start, long size, long threshold)
{
	printf("testbefore\n");
	if (size<=threshold)
	{
		printf("IFTEST -- start p= %p, start d= %d\n",start,*start);
		printf("size= %ld\n",size);
		return linear_sum(start,size);
	}
		printf("testdebut\n");

	long size1=(size)/2;
	long size2=size-size1;
	long mid=(long)start+size1;
	unsigned long s1, s2;
	void * work_result;
	
	printf("START -- start p= %p, start d= %d\n",start,*start);

	struct thread_data data1;
	data1.start=start;
	printf("DATA1 -- start p= %p, start d= %d\n",data1.start,*(data1.start));
	data1.size=size1;
	data1.threshold=threshold;
	
	struct thread_data data2;
	data2.start=(unsigned char *)mid;
	printf("DATA2 -- start p= %p, start d= %d\n",data2.start,*(data2.start));
	data2.size=size2;
	data2.threshold=threshold;

	pthread_t thr1;	

	if (pthread_create(&thr1,NULL,worker,&data1)!=0)
	{
		printf("create error\n");
		exit(1);
	}

	//s2=linear_sum(data2.start,data2.size);
	s2=(unsigned long)(dnc_sum(data2.start,data2.size,data2.threshold));

	if (pthread_join(thr1,&work_result)!=0)
	{
		printf("join error\n");
		exit(1);
	}

	s1=(unsigned long)work_result;
	printf("THREAD END -- start p= %p, start d= %d, s1= %ld\n",data2.start,*(data2.start),s1);

	return (s1+s2);
}

// Counter of threads.
atomic_int thread_count = 1;

// The thread function.
void * worker(void *arg)
{
	// Increment the counter of threads.
	atomic_fetch_add(&thread_count, 1);

	// TODO
	// - Get the thread data passed as parameters.
	// - Call dnc_sum().
	//   (It may execute recursively another thread.)
	// - Store the result in the 'sum' field.
	// - Return from the function.

	struct thread_data *rdata=arg;
	printf("WORKER CALL __________________________\n");
	unsigned long sum=dnc_sum(rdata->start,rdata->size,rdata->threshold);
	return (void *)sum;
	
}

int main(int argc, char **argv)
{
	// Get the arguments.
	// argv[1] = array_size = the size of the array of bytes (greater than 63).
	// argv[2] = thread_number = the minimum number of threads (between 1 and 16).
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

	// Allocate the array of bytes.
	unsigned char *bytes = malloc(array_size);
	if (bytes == NULL)
		errx(EXIT_FAILURE, "Not enough memory!\nTry to reduce the size of the array.");

#ifdef INITIALIZE_ARRAY
	// Initialize the array.
	printf("Initializing array.. ");
	fflush(stdout);
	for (long i = 0; i < array_size; i++)
		bytes[i] = 1;
	printf("OK\n");
	printf("bytes= %p, bytes[0]= %d\n",bytes,bytes[0]); 
#endif

	// Print the sum and the number of threads.
	printf("Sum................. %lu\n", dnc_sum(bytes, array_size, 1 + array_size / thread_number));
	printf("Number of threads... %i\n", atomic_load(&thread_count));

	// Free the array and exit.
	free(bytes);
	return EXIT_SUCCESS;
}
