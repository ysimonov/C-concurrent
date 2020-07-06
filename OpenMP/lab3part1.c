/* File:      lab3part1.c
 * Purpose:   Estimate pi using OpenMP and a monte carlo method
 * 
 * Compile:   gcc ...
 *            *needs my_rand.c, my_rand.h
 *
 * Run:       ./lab3part1 <number of threads> <number of tosses>
 *
 * Input:     None
 * Output:    Estimate of pi
 *
 * Note:      The estimated value of pi depends on both the number of threads and the number of "tosses".  
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
   #include <omp.h>
#endif
#include "my_rand.h"
#include "timer.h"

/*---------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   double pi_estimate;
   int thread_count;
   long long int number_in_circle;
   long long int number_of_tosses;
   
	double start, finish, elapsed;

   thread_count = strtol(argv[1], NULL, 10);  
   number_of_tosses = strtoll(argv[2], NULL, 10);
  
   number_in_circle = 0;

//	printf("thread = %d\n",omp_get_thread_num());

	GET_TIME(start);
#	pragma omp parallel \
	num_threads(thread_count) \
	shared(number_of_tosses) \
	reduction(+:number_in_circle) \
	default(none)
   {
#		ifdef _OPENMP
      	int my_rank = omp_get_thread_num();
#		else
			int my_rank = 0;
			int thread_count = 1;
#		endif
		long long int toss;
		double x, y, distance_squared;
      unsigned seed = my_rank + 1;
		// printf("thread = %d\n",omp_get_thread_num());
#		pragma omp for \
		private(x,y,distance_squared) 
      for(toss = 0; toss < number_of_tosses; toss++) {
			// printf("toss = %d, thread = %d\n",toss, omp_get_thread_num());
         x = 2*my_drand(&seed) - 1; 
         y = 2*my_drand(&seed) - 1; 
         distance_squared = x*x + y*y; 
         if (distance_squared <= 1) {				
				number_in_circle++; 
			}
      }
   }
	GET_TIME(finish);
	elapsed = finish - start; 
	printf("The code in the parallel region with %d thread(s) took %e seconds\n", thread_count, elapsed);

//	printf("thread = %d\n",omp_get_thread_num());

   pi_estimate = 4*number_in_circle/((double) number_of_tosses);
   printf("Estimated pi: %e\n", pi_estimate);

   return 0;
}
