/*
 Author: Thilina Athukorala 
 Class Name: c_queue
 Purpose: Contains the core functions of the programme which handles the queue of customers.
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "c_queue.h"
#include "mutexList.h"




/*
 NAME: createQueue
 PURPOSE: To initialize the variables used in Queue struct.
 IMPORTS: none
 EXPORTS: returns the initialized struct pointer.
 */
Queue* createQueue() {
     Queue* queue = (Queue*) malloc(sizeof(Queue));

         queue->MAX_CUSTOMERS =10;
         queue->queueCount=0;
         queue->front = NULL;
         queue->rear = NULL;
         queue->fileEnd=false;


    
return queue;
}



bool isQueueFull(Queue *queue) {
    if(getQueueSize(queue)==queue->MAX_CUSTOMERS)
    {
        return true;
    }else{
        return false;
    }
}

/*Function to check if the queue is empty*/ 
bool isQueueEmpty( Queue* queue) {

    if( queue->queueCount ==0)
    {
      return true;
    }
    else{
          return false;
    }
}

/*
 NAME: enqueue
 PURPOSE: To add a new customer struct variable to the queue.
 IMPORTS: customer id, service type related to it, with arrival time and queue pointer.
 EXPORTS: none.
 */
void enqueue(int id, char service, char* arrival, Queue* queue) {
    
    
 Customer* newNode = (struct Customer*) malloc(sizeof( Customer));

    
    newNode->customer_id = id;
    newNode->service_type= service;
  
   sprintf(newNode->arrival_time,"%s",arrival);
    newNode->next = NULL;

    if(isQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    }
    else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
     queue->queueCount++;

}


/*
 NAME: dequeue
 PURPOSE: To remove a customer struct variable from the queue.
 IMPORTS: queue pointer.
 EXPORTS: Pointer to the removed customer struct variable.
 */
 Customer* dequeue(Queue* queue) {

 Customer* temp;

    
    if(queue->front == queue->rear) {
        temp = queue->front;
        queue->front = queue->rear = NULL;
        queue->queueCount--;
         return temp;
          
              
    }
    else {
        temp = queue->front;
        queue->front = queue->front->next;
        queue->queueCount--;
     return temp;
      
        
    }

    

   
}


/*
 NAME: getQueueSize
 PURPOSE: To get the number of elements in queue.
 IMPORTS: queue pointer.
 EXPORTS: number of elements in queue.
 */

int getQueueSize(Queue* queue)
{
    int queueSize=0;
	    if(isQueueEmpty(queue)) {
        queueSize= 0;
    
    }else{
	 struct Customer* current = queue->front;
    while(current != NULL) {
        queueSize++;
        current = current->next;
    }
 
	}
return queueSize;
}


		   
	

	

















