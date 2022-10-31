/* Show memory usage for each PID */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <signal.h>
#include <errno.h>

#include "process.h"

//gloabal variables

long Hertz=0;
long uptime=0;
Process_t* p = NULL;




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

char * concat(char* s1, char c){
    char * res=(char*) malloc((strlen(s1)+2)*sizeof(char));
    int i;
    for(i=0;i<strlen(s1);i++){
        res[i]= s1[i];
    }
    res[strlen(s1)]=c;
    res[strlen(s1)+1]='\0';
    return res;
}

char** split_string(char *line, int* s) {

    char** res=(char **)malloc(100*sizeof(char*));

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
    tmp = strtok(NULL, ")");

    char * d=concat(tmp,')'); 
    //il comm puo avere spazi quindi essendo il secondo token ci arrangiamo direttamente cosi. 

    res[index]=d;
    index++;

    for (;;) {
        tmp = strtok(NULL, delimiter);
        if (tmp == NULL){
            *s=index; //size della res
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


void processdir(const struct dirent *piddir)
{

    char *lines = NULL;
    size_t size;
    
    char filename[1024];
    sprintf(filename, "/proc/%s/stat", piddir->d_name);
    FILE *f = fopen(filename, "r");


    char** data;
    int s=0;

    if(getline(&lines, &size, f) != -1){
        data=split_string(lines, &s);
    }

    //calcoli temporali per CPU Usage
    float total_time= atoi(data[13])+atoi(data[14])+atoi(data[15])+atoi(data[16]);
    float seconds= uptime - (atoi(data[21])/Hertz);


    //values
    
    int pid = atoi(piddir->d_name);
    char* status = data[2];
    float cpu_usage= 100 *( (total_time/Hertz)/seconds);
    float vsize = atoi(data[22])/1024;
    char* command= data[1];
    insert(&p,pid,status,cpu_usage,vsize,command);


    fclose(f);
    
}

 int main(int argc, char *argv[]){
    
    //variabili inizializzate per leggere /proc
    DIR *procdir;
    struct dirent *procentry;


    //variabili inizializzate per calcolo CPU usage
    Hertz= sysconf(_SC_CLK_TCK);
    uptime=get_uptime(); 
    if(uptime==-1){ 
        return -1;
    }

    procdir = opendir("/proc");
    if (procdir == NULL) {
        perror("Could not open directory /proc");
        return 1;
    }
    
    //da rendere piu carino
    printf("PPID\tSTATE\tCPU_USAGE(%%)\tMEMORY USAGE(kb)\tCOMMAND\n");

    //legge /proc per ogni linea
    while(1) {

        procentry = readdir(procdir);
        if (procentry == NULL) {
            break;
        }

        // il nome di ogni processo e' il PID che coincide con un codice numerico diverso da 0       
        if (!fnmatch("[1-9]*", procentry->d_name, 0)) { 
             processdir(procentry);
        }
    }
    print(p);

    char* command=(char*)malloc(20*sizeof(char));
    char* comm=(char*) malloc(10*sizeof(char));
    int pid;
    while(1){
        printf("type help for usage or write your command: ");
        fgets(command,20, stdin);
        int state = sscanf(command,"%s %8d", comm,&pid);
        if(state == 1){
            if(strcmp("help",comm)==0){
                printf("\nUSAGE:\nquit - Exit the program\nkill [pid] - kill a running process with pid [pid]\nterminate [pid] - kill a suspended process with pid [pid]\nsuspend [pid] - suspend a running process with pid [pid]\nresume [pid] - resume a suspended process with pid [pid]\n\n");
            }
            else if(strcmp("quit",comm)==0){
                return 0;
            }
            else{
                printf("%s non è un comando valido: help per l' elenco dei comandi\n",command);
            }
            
        }
        else if(state == 2){

            char* stato=states(pid,p);
            printf("il processo %d ha stato: %s! \n",pid,stato);


            if(strcmp("suspend",comm)==0){

                if(*stato=='R'){
                    int res = kill((pid_t)pid,SIGSTOP);
                    if(res == -1){
                        printf("error in SIGSTOP %s\n", strerror(errno));
                    }
                    else{       
                        printf("Eseguita la %s con pid %d! \n",comm,pid);
                    }
                }
                else{
                    printf("Quindi non si puo sospendere\n");
                }
            }
            else if(strcmp("terminate",comm)==0){
                if(*stato=='S'){                              
                    int res =kill((pid_t)pid,SIGTERM);
                    if(res == -1){
                        printf("error in SIGTERM %s\n", strerror(errno));
                    }
                    else{  
                    printf("eseguita la %s con pid %d! \n",comm,pid);
                    }
                }   
                else{
                    printf("Quindi non si puo terminare\n");
                }
                
            }
            else if(strcmp("resume",comm)==0){
                if(*stato=='S'){
                    int res = kill((pid_t)pid,SIGCONT);
                    if(res == -1){
                        printf("error in SIGCONT %s\n", strerror(errno));
                    }
                    else{  
                        printf("eseguita la %s con pid %d! \n",comm,pid);
                    }  
                }
                else{
                    printf("Quindi non si puo resumare\n");
                }
            }
            else if(strcmp("kill",comm)==0){
                int res = kill((pid_t)pid,SIGKILL);
                if(res == -1){
                    printf("error in SIGKILL %s\n", strerror(errno));
                }  
                else{  
                    printf("eseguita la %s con pid %d! \n",comm,pid);
                }
            }
            else{
                printf("%s %d non è un comando valido: help per l' elenco dei comandi\n",command,pid);
            }
        }
        else{
            printf("C hai messo troppa roba o non ci hai scritto nulla\n");
        }
    }
     
}
