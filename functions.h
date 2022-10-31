#include "process.h"

char* states(int pid, Process_t *process);
char * concat(char* s1, char c);
char** split_string(char *line, int* s);
long get_uptime();
