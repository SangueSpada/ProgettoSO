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
    
    /* Construct full path of the file containt memory-info if this PID */
    snprintf(path, BUFSZ, "/proc/%s/status", piddir->d_name);
    
    /* Open the file */
    pidmemfile = fopen(path, "rt");

    /* Read line-by-line until we found the line we want */
    while (fgets(line, BUFSZ, pidmemfile) != NULL) {
        memstr = strstr(line, MEM_FIELD_NAME);
        if (memstr != NULL) {  /* Found our line */
            memstr += offset;
            while (*memstr == ' ' || *memstr == '\t') {
                if (*memstr == '\0') {
                    fprintf(stderr, "unexpected error in %s.\n", path);
                    exit(1);
                }
                ++memstr;
            }
            printf("PID %s: %s", piddir->d_name, memstr);
            break;
        }
    }
    fclose(pidmemfile);
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

     while(1) {
         procentry = readdir(procdir);
         if (procentry == NULL) {
            perror("Could not read procentry");
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
     return 0;
}