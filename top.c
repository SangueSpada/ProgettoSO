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

    //printf("%s\n",piddir->d_name);
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
    const char* pid = piddir->d_name;
    char* status = data[2];
    float cpu_usage= 100 *( (total_time/Hertz)/seconds);
    float vsize = atoi(data[22])/1024;
    char* command= data[1];


    printf("%s\t%s\t%-7.2f%%\t%.1fkb\t\t%s\n", pid,status,cpu_usage,vsize,command);

    fclose(f);
    
}

int main(int argc, int* argv) {
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
    printf("PPID\tSTATE\tCPU_USAGE(%%)\tMEMORY USAGE(kb)\tCOMMAND\n");

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
    while(1){
        printf("SENTI ALLORA PRATICAMENTE SCRIVI QUA o typpa help: ");
        fgets(command, sizeof command, stdin);
        int state = sscanf(command,"%s %d", comm,pid);
        if(state == 1){
            if(strcmp("help",comm)==0){
            printf("\nUSAGE:\nquit - Exit the program\nkill [pid] - kill a process with pid [pid]\nterminate [pid] - terminate a process with pid [pid]\nsuspend [pid] - suspend a process with pid [pid]\nresume [pid] - resume a process with pid [pid]\n\n");
            }
            else if(strcmp("quit",comm)==0){
            return 0;
            }
            else{
                printf("%s non è un comando valido: help per l' elenco dei comandi\n",command);
            }
            
        }
        else if(state == 2){
            if(strcmp("suspend",comm)==0){
                printf("eseguita la %s con pid %d! \n",comm,*pid);
            }
            else if(strcmp("terminate",comm)==0){
                
            }
            else if(strcmp("resume",comm)==0){
                printf("eseguita la %s con pid %d! \n",comm,*pid);
            }
            else if(strcmp("kill",comm)==0){

                printf("eseguita la %s con pid %d! \n",comm,*pid);
            }
            else{
                printf("%s %d non è un comando valido: help per l' elenco dei comandi\n",command,*pid);
            }
        }
        else{
            printf("Too few/many arguments\n");
        }
    }
     
}
