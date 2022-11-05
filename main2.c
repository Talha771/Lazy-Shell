    #include <stdio.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <fcntl.h>
    #include <signal.h>
    #include "./parse.c"
    #include <sys/resource.h>
    struct process{
    int pid;
    char * name; 
    };
    struct process bg_arr[10];
    int bg_process_counter=0;
    int bg_util=0;
    void handler(){
    int wstat;
    pid_t	pid;
    pid = wait3 (&wstat, WNOHANG, (struct rusage *)NULL );
    if (pid==0){
        return;
    }
    if (pid>0){
        for (int i =0 ; i<bg_util;i++){
            if (bg_arr[i].pid==pid){
                if (i==0){
                    bg_util--;
                }
                for (int j=i;j<bg_util;j++){
                    bg_arr[j-1]=bg_arr[j];
                }
                bg_util--;
            }
        printf("Process %d is now closed",pid);
    }
    }
    }
    int main(){
    int built_in_cmd_flag=0;
    int *number;
    parseInfo* cmd;
    char* new_arg[12];
    char hist_arr[10][1024];
    int hist_counter=0;
    int hist_util=0;
    while (1){
    char input[1024];
    printf("\n Prompt>");
    fgets(input,81,stdin);
    cmd = parse(input); 
    hist_counter++;
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
    if (strncmp(input,"kill",4)==0){
        int x = atoi (&input[5])-1;
        int pid2 = bg_arr[x].pid;
        for (int j=x;j<bg_util;j++){
                    bg_arr[j-1]=bg_arr[j];
                }
                bg_util--;

        // cmd->CommArray->VarList[19at("%",(&input[5]));
        // execvp(cmd->CommArray->VarList[0],cmd->CommArray->VarList);
    }
    if (strncmp(input,"history", 7) ==0 ){
        for (int i=0; i<hist_util;i++){
            printf("%s", hist_arr[i]);
        }
        built_in_cmd_flag=1;
    }
    if (strncmp(input,"exit",4)==0){
        break;
        built_in_cmd_flag=1;
    }        
    if (strncmp(input,"jobs", 4) ==0 ){
        printf("Number  ProcessID  Name \n");
        for (int i =0; i<bg_util;i++){
            printf("[%d]    [%d]        %s \n",i+1, bg_arr[i].pid, bg_arr[i].name );
        }
        built_in_cmd_flag=1;
    }
    int pid= fork();
    if (pid==0){
        if (cmd->boolBackground){
            int out = open("/dev/null", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR |O_APPEND);
            dup2(out,1);
        }
        if (cmd->boolOutfile){
            int out = open(cmd->outFile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR |O_APPEND);
            dup2(out,1);
        }
        if (cmd->boolInfile){
            int in = open(cmd->inFile, O_RDONLY);
            dup2(in,0);
        }
        if (built_in_cmd_flag==0){
        execvp(cmd->CommArray->VarList[0],cmd->CommArray->VarList);
        }
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
        signal(SIGCHLD,handler);
        printf("\n parent is running");
    }

    }
    return 1;
    }
