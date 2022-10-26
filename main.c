#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parse.c"
#include <sys/types.h>
#include <unistd.h>
int main (){
    while (1){
        printf("input>");
        char x[81];
        fgets(x,81,stdin);
        parseInfo* cmd; 
        cmd= parse(x);
        print_info(cmd);    
        int pid = fork();
        if (pid == 0){
            printf("child is running");
        }
        else { 
            printf("parent is runnign");
        }
    }
    return 1; 
}