/*
 Author: Thilina Athukorala 
 Class Name: mutexList
 Purpose: Contains mutex locks and conditions which each function uses globally to prevent race conditions and 
 protect mutual exclusion.
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "mutexList.h"


/*Initiate all mutex*/
     pthread_mutex_t queue_mutex =PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t log_mutex =  PTHREAD_MUTEX_INITIALIZER;
   pthread_cond_t queue_not_full = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t queue_not_empty =  PTHREAD_MUTEX_INITIALIZER;

  void destroyAllMutex()
  {
   pthread_mutex_destroy(&queue_mutex);
   pthread_mutex_destroy(&log_mutex);
   pthread_cond_destroy(&queue_not_full);
   pthread_cond_destroy(&queue_not_empty);

  }
    


