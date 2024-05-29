/*
 Author: Thilina Athukorala 
 Curtin ID: 21038875
 Class : main
 PURPOSE: Handles all the main functions required to fill the output log of teller functions completed by threads.
 IMPORTS: Command line arguments for File name, Size of customer queue, Periodic time for adding customers, Time duration to serve withdrawal
 Time duration to serve deposit, Time duration to give information.
 EXPORTS: return '0' at the end 
 ASSERTIONS:  
 POST: All customers will be served.
 MODIFIED : 2023-05-06
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "readFile.h"
#include "c_queue.h"
#include "mutexList.h";
#include "tellerProcess.h"



/*Declaring method to log termination of a teller*/
 void logTermination(int tellerTotal, int tellerIndex,  char* termination, char* initiation);

/*Declaring method to log termination of all tellers*/
 void logStatistics(int tellerTotal[], int totalServed);

/*Declaring method to remove a customer from the queue*/
 Customer* removeFromQueue(Queue* queue);





int main(int argc, char* argv[])
{

 	if(argc<7)
	{

		     printf("\nInsufficient data! Please enter the following arguments with run command to continue \n");
             printf("\n<File name to execute>  <Size of customer queue>  <Periodic time for adding customers>  <Time duration to serve withdrawal>  <Time duration to serve deposit>  <Time duration to give information>\n\n");
			

			 return 0;
	}   



    pthread_t queue_thread;
    pthread_t teller_threads[4];
    pthread_t allThreadID[500]={0};
    char teller_initiation[4][15];
    char teller_names[4][20];
    Queue* queue = createQueue();
    int i;
    time_t now = 0;
    struct tm *LCLTime=NULL;
    int tellerTotal[4] ={0};
    int threadStatus[4] ={0};
    int allThreads[500];
    Teller* alltellers[4] ;


    char *termination_time[4];

    int totalServed=0;
    int withdrawal;
    int deposit;
    int information;

/*Checking wether the file is valid*/
    fileData* filedata = initCustomer();
       FILE *fp;

     fp = fopen(argv[1], "r"); 

           if (fp == NULL)
            {

                    printf("\nError opening file or file not found please enter valid data\n");
                    
                    return 0;
       
            }
     
fclose(fp);
    
   /*assign all command line arguments to the relevant variables */
       int queueSize = atoi(argv[2]);
       int customerInPeriod = atoi(argv[3]);
       withdrawal = atoi(argv[4]);
       deposit = atoi(argv[5]);
       information = atoi(argv[6]);

       filedata->queueSize = queueSize;
       filedata->queue = queue;
       filedata->customerInPeriod = customerInPeriod;

       /*Add all the teller names*/
       strcpy(teller_names[0], "Teller: 1");
       strcpy(teller_names[1], "Teller: 2");
       strcpy(teller_names[2], "Teller: 3");
       strcpy(teller_names[3], "Teller: 4");
       sprintf(filedata->fileName, "%s", argv[1]);



   /*Create thread for adding customers periodically by calling customer() method in readFile.c*/
    pthread_create(&queue_thread, NULL, customer, filedata);
    
       
        
   /*loops until all customers of the file are read and the queue is empty too */   
 while(!(queue->fileEnd) || (queue->queueCount !=0))
 {
         
         
         for (i = 0; i < 4; i++) 
            {  

               if((queue->queueCount !=0))

                 {
                       /*assign the relevant values to the Teller struct fields*/ 
                       alltellers[i] = initTellers();
                       alltellers[i]->depositDuration= deposit;
                       alltellers[i]->informationDuration = information;
                       alltellers[i]->withdrawalDuration = withdrawal;

                       /*Remove a customer from the queue and assign to the field in Teller*/
                       alltellers[i]->customer = removeFromQueue(queue);
            
                            /*assign the name to Teller struct field*/
                             sprintf(alltellers[i]->tellerName, teller_names[i]);

                            /*forming 4 threads for the 4 tellers using the loop */ 
                            if((pthread_create(&teller_threads[i], NULL, teller, alltellers[i]))==0)

                               {
                                      /*noting that the relevant thread is successsfully created*/ 
                                      threadStatus[i]=1;
                                      /*saving thread ID to array*/ 
                                      allThreadID[totalServed] = teller_threads[i];
          
                               }

                             /*If teller has just started serving customers*/
                            if(tellerTotal[i]==0)
                               {
                                     char str[20];
                                     now = time(0);
                                     LCLTime = localtime(&now);
                                     
                                     /*Assign instant time to the string variable and store it in the array */
                                     sprintf(str, "%02d:%02d:%02d", LCLTime->tm_hour, LCLTime->tm_min, LCLTime->tm_sec);
                                     strcpy(teller_initiation[i], str);
                           }
                           /*increase total of the relvant teller*/
                             tellerTotal[i]++;
                             /*increase total of the all tellers*/
                             totalServed++;
            
                               

                 }
            }

      
  
       
    
}



/*Loop through tellers*/
for (i = 0; i < 4; i++) 
{

    /*If the relevant teller is active successfully*/
    if(threadStatus[i]==1)
          {
              /*If the relevant teller has successfully joined*/
             if( pthread_join(teller_threads[i], NULL)==0)
                 {
                   
                      char termination_time[25];
                      now = time(0);
                      LCLTime = localtime(&now);

                       /*Add the instant time to note the teller termination time*/
                      sprintf(termination_time, "%02d:%02d:%02d", LCLTime->tm_hour, LCLTime->tm_min, LCLTime->tm_sec);
                      threadStatus[i] =0;

                       /*log the termination details*/
                      logTermination(i, tellerTotal[i], teller_initiation[i], termination_time);
                 }


           }





 }

/*Loop through extra threads*/
 for (i = 0; i < 500; i++) 
 {

         if(allThreadID[i] !=0)
              {
                   if(allThreadID[i] !=teller_threads[0] && allThreadID[i] !=teller_threads[1] && allThreadID[i] !=teller_threads[2] && allThreadID[i] !=teller_threads[3])
                       {
                            pthread_join(allThreadID[i], NULL);
                             
                           
                       }

               }
 }
  
  /*Log Statictics of all tellers*/
    logStatistics(tellerTotal, totalServed);

      pthread_join(queue_thread, NULL);   
     
     destroyAllMutex();

    printf("\nProgram executed successfully!\n");
        

      return 0;
     }


/*
 NAME: logTermination
 PURPOSE: To log the time with total number of customers served when teller terminates .
 IMPORTS: tellerIndex number, total number of customers served, starting/ending time of teller
 EXPORTS: none
 */
     void logTermination(int tellerIndex, int tellerTotal,  char* initiation, char* termination)
{
         time_t now = 0;
         struct tm *LCLTime=NULL;
         char status_time[25];

     /*Initiate the lock so that no other function can access the log File*/
     pthread_mutex_lock(&log_mutex);

          FILE *fp = fopen("r_log.txt", "a");

    if (fp == NULL) {
        printf("Error opening file.\n");
        
    }

       now = time(0);
       LCLTime = localtime(&now);

    /*Save the instant time which teller is terminated*/
    sprintf(status_time, "%02d:%02d:%02d", LCLTime->tm_hour, LCLTime->tm_min, LCLTime->tm_sec);

    /*Log the details of termination to log file*/
    fprintf(fp, "Termination: Teller-%d\n#served customers: %d\nStart time: %s\nTermination time: %s",(tellerIndex+1), tellerTotal, initiation, termination);
    fprintf(fp, "\n-----------------------------------------------------------------\n");
    fclose(fp);

    /*unlock the mutex so that other functions can access the log file*/
    pthread_mutex_unlock(&log_mutex);

}


/*
 NAME: removeFromQueue
 PURPOSE: To remove a customer from queue when used by a thread.
 IMPORTS: pointer to the queue containing all the customers
 EXPORTS: none
 */

Customer* removeFromQueue(Queue* queue)
{
          Customer* temp;
          time_t now = 0;
          struct tm *LCLTime=NULL;

     /*Initiate the lock so that no other function can access the queue*/
     pthread_mutex_lock(&queue_mutex);

  /*pause */
    while (queue->queueCount==0) 
    {
         pthread_cond_wait(&queue_not_empty, &queue_mutex);
         
    }
              /*Remove one element from queue*/
                temp =dequeue(queue);
                now = time(0);
                LCLTime = localtime(&now);

 /*after removing one element, queue is not full therefore send signal denoting queue isn't full*/
    pthread_cond_signal(&queue_not_full);

     /* Unlock so that other threads can access the queue*/
    pthread_mutex_unlock(&queue_mutex);

    return temp;

}

/*
 NAME: logStatistics
 PURPOSE: To log Statistics of all tellers when customers in file are all served.
 IMPORTS: int array containing total of each teller, total customers served successfully
 EXPORTS: none
 */


     void logStatistics(int tellerTotal[4], int totalServed)
{ 

     /*Initiate the lock so that no other function can access the log File*/
       pthread_mutex_lock(&log_mutex);

          FILE *fp = fopen("r_log.txt", "a");
    if (fp == NULL) {
        printf("Error opening file.\n");
        
    }

/*Log the information*/
    fprintf(fp, "Teller Statistics\n\nTeller-1 served %d customer(s).\nTeller-2 served %d customer(s).",tellerTotal[0], tellerTotal[1]);
    fprintf(fp, "\nTeller-3 served %d customer(s).\nTeller-4 served %d customer(s)",tellerTotal[2], tellerTotal[3]);
    fprintf(fp, "\n\nTotal number of customers: %d", totalServed);
    fprintf(fp, "\n-----------------------------------------------------------------\n");
    fclose(fp);

    /*unlock the mutex so that other functions can access the log file*/
    pthread_mutex_unlock(&log_mutex);







}






