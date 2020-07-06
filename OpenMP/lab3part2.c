/* File:     lab3part2.c
 * Purpose:  Compute a definite integral using the trapezoidal rule and OpenMP.
 *
 * Input:    a, b, n
 * Output:   Estimate of the integral from a to b of f(x) using the trapezoidal rule and n trapezoids.
 *
 * Compile:  gcc ...
 * Usage:    ./lab3part2 <number of threads>
 *
 * Note:  
 *    1.  f(x) is hardwired
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#ifdef _OPENMP
 #include <omp.h>
#endif

double start, finish, elapsed; // timing variables

double f(double x);    /* Function we're integrating */
double Trap(double a, double b, int n, int thread_count);

int main(int argc, char* argv[]) {
   double  global_result = 0.0;  /* Store result in global_result */
   double  a, b;                 /* Left and right endpoints      */
   int     n;                    /* Total number of trapezoids    */
   int     thread_count;

   thread_count = strtol(argv[1], NULL, 10);
   printf("Enter a, b, and n\n");
   scanf("%lf %lf %d", &a, &b, &n);

   global_result = Trap(a, b, n, thread_count);

   printf("With n = %d trapezoids, our estimate\n", n);
   printf("of the integral from %f to %f = %.14e\n",
      a, b, global_result);
   return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input arg:   x
 * Return val:  f(x)
 */
double f(double x) {
   double return_val;

   return_val = x*x*x*x;
   return return_val;
}  /* f */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Use trapezoidal rule to estimate definite integral
 * Input args:  
 *    a: left endpoint
 *    b: right endpoint
 *    n: number of trapezoids
 * Return val:
 *    approx:  estimate of integral from a to b of f(x)
 */
double Trap(double a, double b, int n, int thread_count) {
   double  h, approx;
   int  i;

   h = (b-a)/n; 
   approx = (f(a) + f(b))/2.0; 

	GET_TIME(start);
#	pragma omp parallel for \
	private(i) \
	shared(n,h,a) \
	reduction(+:approx) \
	num_threads(thread_count) \
	default(none)
   for (i = 1; i <= n-1; i++)
     approx += f(a + i*h);
	GET_TIME(finish);
	elapsed = finish - start;
	printf("The code in the parallel region with %d thread(s) took %e seconds\n", thread_count, elapsed);

   approx = h*approx; 

   return approx;
}  /* Trap */
