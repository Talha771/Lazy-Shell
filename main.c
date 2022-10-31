#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include "./parse.c"
int main (){
    int process_counter=0;
    int hist_util =0;
    char* cmd_array[10];
    int pid_arr_counter= 0;
    int pid_arr[10];
    while (1){
        printf("\n \ninput>");
        char x[81];
        fgets(x,81,stdin);
        char* temp_string = malloc(sizeof(x));
        strcpy(temp_string,x);
        parseInfo* cmd; 
        cmd= parse(x);
        // print_info(cmd);    
        if (process_counter==10){
            process_counter=0;
            hist_util=10;
        }
        if (strncmp(x,"history", 7) ==0 ){
            printf ("%d \n",hist_util);
            for (int i =0;i<hist_util;i++){
                printf("%s",cmd_array[i]);
            }
        }
        if (strncmp(x,"exit", 4) ==0 ){
            break;
        }
        if (hist_util==10){
            free(cmd_array[process_counter]);
        }   
        cmd_array[process_counter]=temp_string;
        process_counter+=1;
        hist_util+=1;
        int pid = fork();
        if (pid == 0){
            if (cmd->boolBackground){
                int out = open("/dev/null", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR |O_APPEND);
                dup2(out,1);
            }
            if (cmd->boolInfile){
                int in = open(cmd->inFile, O_RDONLY);
                dup2(in,0);
            }
            if (cmd->boolOutfile){
                int out = open(cmd->outFile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR |O_APPEND);
                dup2(out,1);
            }
            execvp(cmd->CommArray->VarList[0],cmd->CommArray->VarList);
        }
        else { 
            if (cmd->boolBackground!=1){
                wait(NULL);
            }
            else{
                pid_arr[pid_arr_counter]=pid;
                pid_arr_counter++;
                printf("%d",pid_arr[pid_arr_counter]);
            }
            printf("\n parent is running \n");
        }
    }
    return 1; 
}