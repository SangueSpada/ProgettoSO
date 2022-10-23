#include <stdio.h>
#include <unistd.h>


int main(int argc, int* argv){
    printf("STARTING IN 3 SECONDS\n");
    sleep(3);
    while(1){
        printf("doing stuff\n");
        sleep(2);
    }

}