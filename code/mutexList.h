#include <pthread.h>
#ifndef MUTEXLIST
#define MUTEXLIST

  extern  pthread_mutex_t queue_mutex;
   extern pthread_mutex_t log_mutex;
  extern  pthread_cond_t queue_not_full;
  extern  pthread_cond_t queue_not_empty;

void destroyAllMutex();
#endif