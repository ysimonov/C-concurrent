/* File:     
 *     lab2part4.c
 *
 * Purpose: A simple producer consumer exmple
 *
 * Input: none
 *
 * Output: none
 *
 * Compile:  
 *    gcc ...
 * Usage:
 *    ./lab2part4
 *
 */

#include <stdio.h>
#include <pthread.h>

#define MAX 10000			/* Numbers to produce */
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;

void *producer(void *ptr);
void *consumer(void *ptr);

int main(int argc, char **argv) {
  pthread_t pro, con;

  // Initialize the mutex and condition variables
  pthread_mutex_init(&the_mutex, NULL);	 // Initialize mutex
  pthread_cond_init(&condc, NULL);		/* Initialize consumer condition variable */
  pthread_cond_init(&condp, NULL);		/* Initialize producer condition variable */

  // Create the threads
  pthread_create(&con, NULL, consumer, NULL);
  pthread_create(&pro, NULL, producer, NULL);

  // Join the threads
  pthread_join(con, NULL); // waits for the thread specified by con to terminate. On success returns 0
  pthread_join(pro, NULL); // waits for the thread specified by pro to terminate.

/*

       Failure to join with a thread that is joinable (i.e., one that is not
       detached), produces a "zombie thread".  Avoid doing this, since each
       zombie thread consumes some system resources, and when enough zombie
       threads have accumulated, it will no longer be possible to create new
       threads (or processes).

*/

  // Cleanup
  pthread_mutex_destroy(&the_mutex);	/* Free up the_mutex */
  pthread_cond_destroy(&condc);		    /* Free up consumer condition variable */
  pthread_cond_destroy(&condp);		    /* Free up producer condition variable */
  
  printf("Final value in buffer = %d\n",buffer); 
  return 0;
}

void *producer(void* ptr) {
  int i;

  for (i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&the_mutex);	// locking mutex        
    while (buffer != 0)		                
      pthread_cond_wait(&condp, &the_mutex); // used to block on a producer condition variable
    buffer = i; // buffer stores the index
	printf("buffer value of the producer= %d\n",buffer);
    pthread_cond_signal(&condc); // function shall unblock at least one of the threads that are blocked on a consumer condition variable    
    pthread_mutex_unlock(&the_mutex); // releasing mutex	    
  }
  return NULL;
}

void *consumer(void* ptr) {
  int i;

  for (i = 1; i <= MAX; i++) {
    pthread_mutex_lock(&the_mutex);
    while (buffer == 0)
      pthread_cond_wait(&condc, &the_mutex);
    buffer = 0;
    pthread_cond_signal(&condp);
    pthread_mutex_unlock(&the_mutex);
  }
  return NULL;
}


