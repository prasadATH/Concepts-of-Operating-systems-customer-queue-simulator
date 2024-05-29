#include <pthread.h>
#include <stdbool.h>
#include "mutexList.h"
#ifndef C_QUEUE_H
#define C_QUEUE_H

/*
 NAME: Customer (struct)
 PURPOSE: To store all data related to a customer read from the file.
 */
typedef struct Customer {
    int customer_id;
    char service_type;
    char arrival_time[25];
    struct Customer* next;
}Customer;

/*
 NAME: Queue (struct)
 PURPOSE: To store all data related to the queue of customer.
 */
typedef struct Queue {
    int MAX_CUSTOMERS;
    int queueCount;
    bool fileEnd;

    
 Customer* front;
 Customer* rear;
}Queue;


Queue* createQueue() ;
bool isQueueFull(Queue *queue);
bool isQueueEmpty(Queue* queue);
void nqueue(int id, char service, char* arrival, Queue* queue);
 Customer* dequeue(Queue* queue);
 void displayQueue(Queue* queue);

 int getQueueSize(Queue* queue);

#endif