
#include "functions.h"
#include <sys/sysinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* states(int pid, Process_t *process){
    char file_n[30];
    char n_pid[10];
    char* status;
    char* err = NULL;
    sprintf(n_pid, "%d", pid); //int to string
    int res=sprintf(file_n, "/proc/%d/stat", pid);
    
    FILE *f = fopen(file_n, "r");
    if(f==NULL){
        printf("errore apertura file stat");
        return err;
    }

    res=fscanf(f, "%*d %*s %s", status); //evitiamo il pid e il command
    if(res==-1){
        printf("errore lettura status");
        return err;
    }

    return status;

}



char** split_string(char *line) {

    char** res=(char **)malloc(46*sizeof(char*));

    int index=0;
    const char* delimiter = " ";
    char * tmp;




    tmp = strtok(line, delimiter);
    if (tmp == NULL){
    return NULL;
    } 

    res[index]=tmp;
    index++;
    //printf("%s\n", tmp);
    
    char* p=line;
    char* ress=(char*)malloc(30*sizeof(char));
    int iii=0;
    while(p!=NULL){
        if(*p=='('){
            //tmp = strtok(NULL,"(");
            ress[iii]='(';
            iii++;
            p++;
            while(!(*p==')'&& *(p+1)==' ')){
                ress[iii]=*p;
                iii++;
                p++;

            }
            ress[iii]=')';
            p+=2;
            ress[iii+1]='\0';
            break;
        }
        p++;
    }    


    res[index]=ress;
    index++;

    tmp=strtok(p,delimiter);
    res[index]=tmp;
    index++;

    for (;;) {
        tmp = strtok(NULL, delimiter);
        if (tmp == NULL){
            return res;
        }
        
        res[index]=tmp;
        index++;
    }
}



long get_uptime(){
    struct sysinfo s_info;
    int err= sysinfo(&s_info);
    if(err!=0){
        printf("errore lettura uptime, code= %d\n", err);
        return -1;
    }
    return s_info.uptime;
}
