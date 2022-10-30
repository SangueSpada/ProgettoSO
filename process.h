#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct Process{
  int pid; 
  char* status;
  float cpu_usage;
  float vsize;
  char* command;
  struct Process* next;
}Process_t;

Process_t* init(void);

void insert(Process_t* p, int pid, char* status, float cpu_usage,float vsize, char* command);

Process_t* search(Process_t* p,int pid);

void print(Process_t* p);



