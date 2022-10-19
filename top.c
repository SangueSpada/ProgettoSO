/* Show memory usage for each PID */

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <string.h>

#define BUFSZ 1024
#define MEM_FIELD_NAME "VmSize:"

void processdir(const struct dirent *piddir)
{
    char path[BUFSZ];
    char line[BUFSZ];
    char *memstr;
    FILE *pidmemfile;
    int  offset = strlen(MEM_FIELD_NAME);
    
    char filename[1024];
    sprintf(filename, "/proc/%s/stat", piddir->d_name);
    FILE *f = fopen(filename, "r");

    int unused;
    char comm[1000];
    char state;
    int ppid;
    fscanf(f, "%d %s %c %d", &unused, comm, &state, &ppid);
    printf("%s\t%c\t%s\n", piddir->d_name,state,comm);
    fclose(f);
    
}


int main() 
{
     DIR *procdir;
     struct dirent *procentry;

     procdir = opendir("/proc");
     if (procdir == NULL) {
         perror("Could not open directory /proc");
         return 1;
     }
     printf("PPID\tSTATE\tCOMMAND\n");

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