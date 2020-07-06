/* File:      lab2part1.c
 * Purpose:   Estimate pi using pthreads and monte carlo method
 * 
 * Compile:   gcc ...
 *	         needs my_rand.c, my_rand.h
 * Run:       ./lab2part1 <number of threads> <number of tosses>
 * Input:     None
 * Output:    Estimate of pi
 *
 * Note:      The estimated value of pi depends on both the number of threads and the number of "tosses".  
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_rand.h"
#include <string.h>
#include "timer.h"

#define MR_MULTIPLIER 279470273 
#define MR_INCREMENT 0
#define MR_MODULUS 4294967291U
#define MR_DIVISOR ((double) 4294967291U)

/* Global variables */
int thread_count;
long long int number_in_circle = 0;
long long int number_of_tosses;
double start, finish, elapsed;

/* mutex */
pthread_mutex_t mutex;

/* Serial function */
void Get_args(int argc, char* argv[]);

unsigned my_rand(unsigned* seed_p);
double my_drand(unsigned* seed_p);

/* Parallel function */
void *Thread_work(void* rank);

/*---------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	long i; 
	pthread_t* thread_handles;
	double pi_estimate;

	if(argc != 3) {
		printf("Error. Enter <number_of_threads> <number_of_tosses>\n");
		exit(0);
	} Get_args(argc, argv);

	thread_handles = malloc(thread_count*sizeof(pthread_t));

	pthread_mutex_init(&mutex, NULL); /* initialize mutex */
	
	// -------------------- PARALLEL REGION ----------------------- //

	GET_TIME(start); // start timer

	/* create thread with attribute thread_handle[i], executing the function Thread_work, with rank i */
	for(i = 0; i < thread_count; i++) 
		pthread_create(&thread_handles[i], NULL, &Thread_work, (void*)(long)i); 

	for(i = 0; i < thread_count; i++) 
		pthread_join(thread_handles[i], NULL); /* join corresponding threads */

	GET_TIME(finish); // end timer

	// -------------------END OF PARALLEL REGION-------------------- //

	elapsed = finish - start; // calculate elapsed time
	printf("The code to be timed took %e seconds\n", elapsed);
	
	pi_estimate = 4*number_in_circle/((double) number_of_tosses);
	printf("Estimated pi: %e\n", pi_estimate);

	pthread_mutex_destroy(&mutex); /* destroy mutex */ free(thread_handles); // deallocate pointer
	return 0;
}

/*---------------------------------------------------------------------
 * Function:		Thread_work 
 * Purpose:		Calculate number in circle using monte carlo method
 * In arg:		rank
 * Global in vars:	number_of_tosses, thread_count
 * Global out vars:	number_in_circle
 */

void *Thread_work(void* rank) {
	long my_rank = (long) rank;
	long long int toss;
	long long int local_number_in_circle = 0;
	long long int local_tosses = number_of_tosses/thread_count;
	long long int start = local_tosses*my_rank;
	long long int finish = start+local_tosses;
	double x, y, distance_squared;
    	unsigned seed = my_rank+1;  /* must be nonzero */

	for(toss = start; toss < finish; toss++) {
	
		// printf("%d\n",seed);

		/*Alternative: x = my_drand(&seed) * 2.0 - 1.0 (my_drand is not accurate enough)*/
        x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0; /* x= random between -1 and 1 */
		//x = my_drand(&seed) * 2.0 - 1.0;
		
		/*Alternative: y = my_drand(&seed) * 2.0 - 1.0 (my_drand is not accurate enough)*/
        y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0; /* y= random between -1 and 1 */
		//y = my_drand(&seed) * 2.0 - 1.0;

        distance_squared = x * x + y * y; /* distance_squared = distance squared of dart toss from centre position */

		if (distance_squared <= 1) {
			local_number_in_circle ++; /* if dart falls in unit circle, increment the counter local_number_in_circle */
		}

	}

	/*	CODE BELOW
		1. locking for thread safety (critical section)
		2. Adding local_number_in_circle to (global variable) number_in_circle
		3. Leaving critical section 
	*/

	pthread_mutex_lock(&mutex);  
	number_in_circle += local_number_in_circle; 
	pthread_mutex_unlock(&mutex); 
	
	return 0;
}

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */

void Get_args(int argc, char* argv[]) {
	thread_count = strtol(argv[1], NULL, 10);  
	number_of_tosses = strtoll(argv[2], NULL, 10);
}  /* Get_args */


/* Function:      my_rand
 * In/out arg:    seed_p
 * Return value:  A new pseudo-random unsigned int in the range
 *                0 - MR_MODULUS
 *
 * Notes:  
 * 1.  This is a slightly modified version of the generator in an
 *     old version of the Wikipedia article "Linear congruential 
 *     generator"
 * 2.  The seed_p argument stores the "state" for the next call to
 *     the function.
 * 3.  *seed_p must be nonzero
 * 4.  The value of *seed_p should be set before the first call 
 *     to my_rand, and the value returned from one call should
 *     simply be passed to the next call.
 */
unsigned my_rand(unsigned* seed_p) {
   long long z = *seed_p;
   z *= MR_MULTIPLIER; 
   // z += MR_INCREMENT;
   z %= MR_MODULUS;
   *seed_p = z;
   return *seed_p;
}

/* Function:      my_drand
 * In/out arg:    seed_p
 * Return value:  A new pseudo-random double in the range 0 - 1
 *
 * Note:          See my_rand for info about seed_p
 */
double my_drand(unsigned* seed_p) {
   unsigned x = my_rand(seed_p);
   double y = x/MR_DIVISOR;
   return y;
}

