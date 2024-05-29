#include "c_queue.h"
#ifndef READFILE_H
#define READFILE_H

/*
 NAME: fileData (struct)
 PURPOSE: To store all data related to file to be read.
 */
typedef struct fileData {
  Queue* queue;
  char fileName[20];
  int queueSize;
  int customerInPeriod;


    
}fileData;

 
fileData* initCustomer();
void* customer(fileData*);


void addToQueue(int customer_num,char service_type,  Queue* queue, int userQueueSize,int customerInPeriod);



#endif