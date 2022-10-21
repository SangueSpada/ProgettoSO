/* Show memory usage for each PID */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>SangueSpada/ProgettoSO.git
#define BUFSZ 1024
#define MEM_FIELD_NAME "VmSize:"

long Hertz=0;
long uptime=0;


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




long get_uptime(){

    struct sysinfo s_info;
    int err= sysinfo(&s_info);
    if(err!=0){
        printf("errore lettura uptime, code= %d\n", err);
        return -1;
    }

    return s_info.uptime;


}


char** split_string(char *line, int* s) {

    //char* res[100];
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

void processdir(const struct dirent *piddir)
{

    //printf("%s\n",piddir->d_name);
    char *lines = NULL;
    size_t size;

    int  offset = strlen(MEM_FIELD_NAME);
    
    char filename[1024];
    sprintf(filename, "/proc/%s/stat", piddir->d_name);
    FILE *f = fopen(filename, "r");


    char** data;
    int s=0;

    if(getline(&lines, &size, f) != -1){
        data=split_string(lines, &s);
    }
    

    //printf("size= %d\n",s);
    /*for(int i=0;i<s;i++){

        printf("%s\n",data[i]);

    }*/

    //calcoli temporali per CPU Usage
    float total_time= atoi(data[13])+atoi(data[14])+atoi(data[15])+atoi(data[16]);

    float seconds= uptime - (atoi(data[21])/Hertz);

    
    float cpu_usage= 100 *( (total_time/Hertz)/seconds);





    printf("%s\t%s\t%-7.2f%%\t\t\t%s\n", piddir->d_name,data[2],cpu_usage, data[1]);

    free(data[1]);
    free(data);
    fclose(f);
    
}

void split_cmd_line(char* data,int* pid, char* line, char* delim){
    char* tmp = strtok(line,delim);
    strcpy(data,tmp);
    tmp = (strtok(NULL,delim));
    if(tmp!=NULL ){
    int res=atoi(tmp);
    if(res!=0){
    *pid=atoi(tmp);
    }
    
    }
}


int main(int argc, int* argv) 
{
     DIR *procdir;
     struct dirent *procentry;

     Hertz= sysconf(_SC_CLK_TCK);
     uptime=get_uptime(); //prende inizio dell'OS?

     if(uptime==-1){ 
        return -1;
     }

     procdir = opendir("/proc");
     if (procdir == NULL) {
         perror("Could not open directory /proc");
         return 1;
     }
     printf("PPID\tSTATE\tCPU_USAGE(%%)\t\t\tCOMMAND\n");

     while(1) {
         procentry = readdir(procdir);
         if (procentry == NULL) {
            break;
         }
         /* if the name of an entry in /proc has only digits, then
          * it is a subdirectory containg info about a process,
          * while the name itself is the PID of the process.
          */        
         if (!fnmatch("[1-9]*", procentry->d_name, 0)) {
             processdir(procentry);
         }
     }

     char* command=(char*)malloc(10*sizeof(char));
     char * comm=(char*) malloc(10*sizeof(char));
     int* pid=(int*) calloc(1,sizeof(int));
     *pid=-1;
     while(1){
        printf("SENTI ALLORA PRATICAMENTE SCRIVI QUA o typpa help: ");

        scanf("%[^\n]%*c",command);
        split_cmd_line(comm,pid,command," ");
        //printf("%s ->  %d",comm,*pid);

        if(strcmp("help",comm)==0){
            printf("quit - Exit the program\nkill [pid] - kill a process with pid [pid]\nterminate [pid] - terminate a process with pid [pid]\nsuspend [pid] - suspend a process with pid [pid]\nresume [pid] - resume a process with pid [pid]\n");
        }
        else if(strcmp("quit",comm)==0){
            return 0;
        }
        
        else if(strcmp("suspend",comm)==0)
        {
            if(*pid==-1){
                printf("inserire un pid valido\n");

            }
            else{
           //split_cmd_line(data,input," ");
           printf("eseguita la %s! \n",comm);
            *pid=-1;
            }

        }
        else if(strcmp("terminate",comm)==0)
        {
            if(*pid==-1){
                printf("inserire un pid valido\n");

            }
            else{
           //split_cmd_line(data,input," ");
           printf("eseguita la %s! \n",comm);
            *pid=-1;
            }

        }
        else if(strcmp("resume",comm)==0)
        {
            if(*pid==-1){
                printf("inserire un pid valido\n");

            }
            else{
           //split_cmd_line(data,input," ");
           printf("eseguita la %s! \n",comm);
            *pid=-1;
            }

        }
        else if(strcmp("kill",comm)==0)
        {
            if(*pid==-1){
                printf("inserire un pid valido\n");

            }
            else{
           //split_cmd_line(data,input," ");
           printf("eseguita la %s! \n",comm);
            *pid=-1;
            }

        }
        else{
            printf("%s non è un comando valido: help per l' elenco dei comandi\n",command);
        }
        
        

     }
     
}
