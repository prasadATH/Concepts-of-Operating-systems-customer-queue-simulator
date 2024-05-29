/*
 Author: Thilina Athukorala 
 Class Name: readFile
 Purpose: Contains the core functions of the programme which handles file reading and addition of customers to queue.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "readFile.h"
#include "c_queue.h"
#include "mutexList.h"




/*
 NAME: initCustomer
 PURPOSE: To initialize the variables used in fileData struct.
 IMPORTS: none
 EXPORTS: returns the initialized struct pointer.
 */
 
fileData* initCustomer()
{
    /*Initialize all fields in struct fileData*/
    fileData* filedata = (fileData*) malloc(sizeof(fileData));
    filedata->queue=NULL;
    filedata->queueSize = 0;
    filedata->customerInPeriod=0;
    

    return filedata;
}

/*
 NAME: customer
 PURPOSE: To read customers from file to queue whenever the mutex signal is received.
 IMPORTS: pointer to the struct containing details required to read the file
 EXPORTS: returns the queue struct pointer.
 */
 
void* customer(fileData* allFileData)
{
      FILE *fp;
      int lineNum=0;
      int splitNum=0;
      char buffer[100];
      char tknsplit[5];
      char *token;
      int customer_num=0;
      char service_type;

      Queue* queue = allFileData->queue;

  

    
/*initiate file reader*/
    fp = fopen(allFileData->fileName, "r"); 

           if (fp == NULL)
            {
                    printf("Error opening file\n file:");
       
            }


    while (fgets(buffer, 100, fp) != NULL)
    { 
        splitNum=0;
        token = strtok(buffer, " "); 
        while (token != NULL) { 
            
   strcpy(tknsplit, token);
         /*if it's the first element in line*/
            if(splitNum==0)
            {
              
                 customer_num=atoi(token);
               
            }
 

            /*if it's the second element in line*/
            if(splitNum==1)
            {
                
                 service_type = token[0];
            }
            splitNum++;
            
            token = strtok(NULL, " "); 
        }

 /*function to add customers to queue periodically*/
addToQueue(customer_num, service_type, queue, allFileData->queueSize,allFileData->customerInPeriod);

        lineNum++;


    }

    if (feof(fp)) {
        /*notify to queue that struct that the end of file is reached*/
        queue->fileEnd=true;
      
    }
    /*close file reader*/
    fclose(fp);

return queue;

}


/*
 NAME: addToQueue
 PURPOSE: To add a new customer to queue .
 IMPORTS: customer ID, service type required by customer, queue to add the customer, 
 size of queue defined by user, time period to add a new customer 
 EXPORTS: returns the initialized struct pointer.
 */
void addToQueue(int customer_num,char service_type,  Queue* queue, int userQueueSize, int customerInPeriod)
{

       time_t now = 0;
       struct tm *LCLTime=NULL;
       char arrival_time[25];
       arrival_time[25] = '\0';

 /*Initiate the lock so that no other function can access the queue simultaneously*/
     pthread_mutex_lock(&queue_mutex);

            /*Pause the addition of new customers when queue is full*/
            while (queue->queueCount==userQueueSize) 
            {
                /*waiting until a customer is removed from queue by a teller*/
               pthread_cond_wait(&queue_not_full, &queue_mutex);
            }

/*sleep for the duration user has mentioned*/
sleep(customerInPeriod);

                 /*Get the instant time and save*/
                now = time(0);
                LCLTime = localtime(&now);
                sprintf(arrival_time, "%02d:%02d:%02d", LCLTime->tm_hour, LCLTime->tm_min, LCLTime->tm_sec);

 
        /*add a new customer to queue*/
        enqueue(customer_num, service_type, arrival_time, queue);

/*initiate lock so that no other function can access the log file at this time*/
     pthread_mutex_lock(&log_mutex);

          FILE *fp = fopen("r_log.txt", "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        
    }

    fprintf(fp, "Customer %d: %c\nArrival time: %s",customer_num,service_type,arrival_time);
    fprintf(fp, "\n-----------------------------------------------------------------\n");
    fclose(fp);
  
           /*release the lock giving access to other functions*/
           pthread_mutex_unlock(&log_mutex);


           /*Signal the tellers that the queue is not empty and they can remove customers from queue*/
            pthread_cond_signal(&queue_not_empty);
            /*Release lock aquired for the queue*/
             pthread_mutex_unlock(&queue_mutex);

        /*close file reader*/
  

}









