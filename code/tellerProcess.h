#include "c_queue.h"
#ifndef TELLERPROCESS
#define TELLERPROCESS

/*
 NAME: Teller (struct)
 PURPOSE: To store all data related to the teller assigned to a customer.
 */
typedef struct Teller {
  Customer* customer;
  char tellerName[20];
  int withdrawalDuration;
  int depositDuration;
  int informationDuration;


    
}Teller;

Teller* initTellers();
 void* teller(Teller* customer);
 void logStatus(Customer* customer, char* tellerName, char* teller_status);
#endif