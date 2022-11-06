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

char *sliceString(char *str, int start, int end)
{

    int i;
    int size = (end - start) + 2;
    char *output = (char *)malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++)
    {
        output[i] = str[start];
    }

    output[size] = '\0';

    return output;
}
 
// Above function taken from the internet to help with slicing 
    // jobs is not working properly
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
    int empty_string=0;
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
    if (hist_counter==10){
        hist_counter=0;
    }
    if (strncmp(input," ",1)==0){
        printf("empty");
        empty_string=1;
    }
    else{
        strcpy(hist_arr[hist_counter],input);
    }
    if (hist_util<10 && empty_string==0) {
        hist_util++;
    }
    if (strncmp(input,"help",4)==0){
        printf("command: jobs description: Provides list of background processes and their local process ids\ncommand: cd PATHNAME description: Sets PATHNAME as current working directory");
        printf("\ncommand: history description: Prints list of previously executed commands(Max 10)\ncommand: kill PID description: Terminates process identified locally with PID in jobs list");
        printf("\ncommand: !CMD description: runs the command numbered CMD in command history\ncommand: exit description: Terminates shell if and only if there are no background jobs\ncommand: help description: prints list of built in commands along with their description\n");
    }
    if (strncmp(input,"cd",2)==0){
    
        // char* dir = sliceString(input,1,81);
        // chdir(dir);
    }
    if (strncmp(input,"kill",4)==0){
        int x = atoi (&input[5])-1;
        int pid2 = bg_arr[x].pid;
        kill(pid2,SIGTERM);
        for (int j=x;j<bg_util;j++){
            bg_arr[j-1]=bg_arr[j];
        }
        bg_util--;
        
        // kill(pid2,SIGKILL);
        // cmd->CommArray->VarList[19at("%",(&input[5]));
        // execvp(cmd->CommArray->VarList[0],cmd->CommArray->VarList);
    }
    if (strncmp(input,"cmd",3)==0){
        printf("%s",&input[4]);
        int index = atoi (&input[4]) ;
        // printf("%d", index);
        // cmd = parse(hist_arr[index]);
        parseInfo* new =parse(hist_arr[index]);
        execvp(new->CommArray->VarList[0],new->CommArray->VarList);
        built_in_cmd_flag=1;
    }
    if (strncmp(input,"history", 7) ==0 ){
        for (int i=1; i<hist_util;i++){
            printf("[%d] %s \n",i, hist_arr[i] );
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
    int pid =-1;
    if (empty_string==0){
        pid= fork();
    }
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
        if (execvp(cmd->CommArray->VarList[0],cmd->CommArray->VarList)==-1){
            printf("wrong command muthafuka");
            break;
        }
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
    if (pid < 0){
        continue;
    }
    }
    return 1;}
