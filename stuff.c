#include <stdio.h>
#include <unistd.h>


 int main(int argc, char *argv[]){
    printf("STARTING IN 3 SECONDS\n");
    sleep(3);
    while(1){
        printf("doing stuff\n");
        sleep(2);
    }

}