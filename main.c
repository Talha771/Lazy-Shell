#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include "./parse.c"
    struct process{
        int pid;
        char * name; 
    };
    void handler(){
        printf("zain is loru");
    }
    int main(){
    parseInfo* cmd;
    char* new_arg[12];
    char hist_arr[81][10];
    int hist_counter=0;
    int hist_util=0;
    struct process bg_arr[10];
    int bg_process_counter=0;
    int bg_util=0;
    while (1){
        char input[81];
        printf("\n Prompt>");
        fgets(input,81,stdin);
        cmd = parse(input);    
        for (int i=0;i<cmd->CommArray->VarNum;i++){
            new_arg[i]=cmd->CommArray->VarList[i];
        }
        new_arg[11]=NULL;
        if (hist_counter==10){
            hist_counter=0;
        }
        if (hist_util<10){
            hist_util++;
        }
        strcpy(hist_arr[hist_counter],input);
        hist_counter++;
        if (strncmp(input,"history",7==0)){
            for (int i =0 ;i<hist_util;i++){
                printf("%s \n", hist_arr[i]);
            }
        }
        if (strncmp(input,"jobs", 4) ==0 ){
            for (int i =0; i<bg_util;i++){
                printf("[%d]        %s \n", bg_arr[i].pid, bg_arr[i].name );
            }
        }
        int pid= fork();
        if (pid==0){
            if (cmd->boolOutfile){
                int out = open(cmd->outFile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR |O_APPEND);
                dup2(out,1);
            }
            if (cmd->boolInfile){
                int in = open(cmd->inFile, O_RDONLY);
                dup2(in,0);
            }
            execvp(cmd->CommArray->VarList[0],cmd->CommArray->VarList);
        }
        else{ 

            if (cmd->boolBackground==0){
                wait(NULL);
            }
            else{
                if (bg_process_counter==10){
                    bg_process_counter=0;
                    bg_util==10;
                }
                if (bg_util<10){
                    bg_util++;
                }
                bg_arr[bg_process_counter].name=cmd->CommArray->command;
                bg_arr[bg_process_counter].pid=pid;
                bg_process_counter++;
            }
            printf("parent is running");
        }

    }
    return 1;
    }
