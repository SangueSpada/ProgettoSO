/* Show memory usage for each PID */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

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
    const char delimiter[] = " ";
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

    res[index]=d;
    index++;



    for (;;) {
        tmp = strtok(NULL, delimiter);
        if (tmp == NULL){
            *s=index;
            return res;
        }
        
        res[index]=tmp;
        index++;
        //printf("%s\n", tmp);
    }
}

void processdir(const struct dirent *piddir)
{

    //printf("%s\n",piddir->d_name);
    char *lines = NULL;
    size_t size;
    char path[BUFSZ];
    char line[BUFSZ];
    char *memstr;
    FILE *pidmemfile;
    int  offset = strlen(MEM_FIELD_NAME);
    
    char filename[1024];
    sprintf(filename, "/proc/%s/stat", piddir->d_name);
    FILE *f = fopen(filename, "r");


    char** data;
    int s=0;

    while (getline(&lines, &size, f) != -1) {
        data=split_string(lines, &s);
    }

    //printf("size= %d\n",s);
    /*for(int i=0;i<s;i++){

        printf("%s\n",data[i]);

    }*/




    float total_time= atoi(data[13])+atoi(data[14])+atoi(data[15])+atoi(data[16]);

    float seconds= uptime - (atoi(data[21])/Hertz);

    
    float cpu_usage= 100 *( (total_time/Hertz)/seconds);





    printf("%s\t%s\t%-7.2f\t\t\t%s\n", piddir->d_name,data[2],cpu_usage, data[1]);

    free(data[1]);
    free(data);
    fclose(f);
    
}


int main() 
{
     DIR *procdir;
     struct dirent *procentry;

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
     printf("PPID\tSTATE\tCPU_USAGE(perc.)\t\t\tCOMMAND\n");

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

     char input[10];
     printf("SENTI ALLORA PRATICAMENTE SCRIVI QUA: ");
     while(1){
        scanf("%s",input);
        if(strcmp("quit",input)==0){
            return 0;
        }
        else{
            printf("%s\n",input);
        }

     }
     
}