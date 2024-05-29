/*
 Author: Thilina Athukorala 
 Class Name: tellerProcess
 Purpose: Contains functions which handles the teller functions of customers which require a service.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "tellerProcess.h"
#include "c_queue.h"




/*
 NAME: initTellers
 PURPOSE: To initialize the variables used in Teller struct.
 IMPORTS: none
 EXPORTS: returns the initialized struct pointer.
 */
Teller* initTellers()
{
    Teller* teller = (Teller*) malloc(sizeof(Teller));

    teller->customer=NULL;
    

    return teller;
}

/*
 NAME: tellerProcess
 PURPOSE: To perform the process by teller function according to service mentioned by customer.
 IMPORTS: Teller struct which contains relevant teller information including the time duration for each service.
 EXPORTS: returns a void pointer.
 */

 void* teller(Teller* alldata)
{
        
          Customer* customer = alldata->customer;
          char name[16];
          time_t now = 0;
          struct tm *LCLTime=NULL;
          char status_initiate[12] = "Response";
          char status_complete[12] = "Completion";

          
/*If customer is having the service type cash-Withdrawal*/
    if((customer->service_type =='W'))
    {
          /*Log before initiating service of customer*/
            logStatus(customer, alldata->tellerName, status_initiate);
       
       /*sleep for the duration given in command line argument for cash-Withdrawal*/
            sleep(alldata->withdrawalDuration);

         /*Log after completing service of customer*/
            logStatus(customer, alldata->tellerName, status_complete);
      

/*If customer is having the service type cash-Deposit*/
    }else if((customer->service_type =='D'))
    {

            logStatus(customer, alldata->tellerName, status_initiate);

            /*sleep for the duration given in command line argument for cash-Deposit*/
            sleep(alldata->depositDuration);

            logStatus(customer, alldata->tellerName, status_complete);
      
    }else if((customer->service_type =='I'))
    {

            logStatus(customer, alldata->tellerName, status_initiate);

  /*sleep for the duration given in command line argument for information*/
            sleep(alldata->informationDuration);

            logStatus(customer, alldata->tellerName, status_complete);
      
    }


return NULL;


}


/*
 NAME: logArrival
 PURPOSE: To log the status of a teller when teller receives a customer.
 IMPORTS: customer struct which contains information on the customer, teller information, status of the job.
 EXPORTS: none.
 */
void logStatus(Customer* customer, char* tellerName,  char* teller_status)
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
        /*Get and save instant time*/
         now = time(0);
        LCLTime = localtime(&now);
        sprintf(status_time, "%02d:%02d:%02d", LCLTime->tm_hour, LCLTime->tm_min, LCLTime->tm_sec);


    fprintf(fp, "%s \nCustomer: %d\nArrival time: %s\n%s time: %s",tellerName, customer->customer_id,customer->arrival_time, teller_status, status_time);
    fprintf(fp, "\n-----------------------------------------------------------------\n");
    fclose(fp);

    /*release the lock so that  other function can access the log File*/
    pthread_mutex_unlock(&log_mutex);

}

