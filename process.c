#include "process.h"
#include <string.h>

Process_t* init(void){
    return NULL;
}

void insert(Process_t** p, const int pid, char* status, float cpu_usage, float vsize, char* command){
    Process_t *cur, *prev, *new_node;
    new_node = (Process_t*) malloc(sizeof(Process_t));
    if(new_node == NULL){
        printf("Cannot allocate new nodes");
    }
    
    for(cur = *p, prev=NULL; 
        cur != NULL && cpu_usage > cur->cpu_usage; 
        prev = cur, cur = cur->next);
    new_node->cpu_usage = cpu_usage;
    new_node->pid = pid;
    new_node->status = (char*) malloc(strlen(status)+1);
    strcpy(new_node->status,status);
    new_node->vsize = vsize;
    new_node->command = (char*) malloc(strlen(command)+1);
    strcpy(new_node->command,command);
    
    new_node->next = cur;
    if(prev == NULL) {
        *p = new_node; //se e il primo elemento allora new node e la testa della lista
    }
    else{
        prev->next = new_node;
    }
        

}

Process_t* search(Process_t* p,int pid){
    
    Process_t *cur;

    for(cur = p; cur != NULL; cur = cur->next){
        if(cur->pid == pid) return cur;
    }
    return NULL;
}


void print(Process_t* p){
    
    Process_t *cur;
    if(p == NULL){
        printf("ERRORE P = null\n");
    }
    for(cur = p; cur != NULL; cur = cur->next){

        printf("%d\t%s\t%-7.2f%%\t%.1fkb\t\t%s\n", cur->pid,cur->status,cur->cpu_usage,cur->vsize,cur->command);
        
    }

}