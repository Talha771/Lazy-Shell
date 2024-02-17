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
        struct process *bg_arr1 ;
        int bg_process_counter=0;
        int bg_util=0;
        void remove_at_kill(char* input){
        int x= atoi(&input[5]);
        x--;
        int pid = bg_arr[x].pid;
        kill(pid,SIGKILL);
        for (int j=x;j<bg_util;j++){
                bg_arr[j-1]=bg_arr[j];
            }
            bg_util--;
            bg_process_counter--;
            return;
        }
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
                    for (int j=i;j<bg_util;j++){
                        bg_arr[j-1]=bg_arr[j];
                    }
                    bg_util--;
                    bg_process_counter--;
                }
            printf("Process %d is now closed",pid);
                }
            }
        }
        int main(){
            char* new_arg[12];
            char hist_arr[10][1024];
            int hist_counter=0;
            int hist_util=0;
            while (1){
        
            parseInfo* cmd;
            int empty_string=0;
            int internal_cmd_flag=0;
            char input[1024];
            printf("\n Prompt>");
            fgets(input,81,stdin);
            cmd = parse(input);
            if (cmd->CommArray->command!=NULL){
                hist_counter++;   
            if (hist_counter>8){
                hist_counter=0;
            }   
            strcpy(hist_arr[hist_counter],input);
            if (hist_util<10) {
                hist_util++;
            }
            }
            if (strncmp(input,"help",4)==0){
                printf("what it does: jobs description: Provides list of background processes and their local process ids\ncommand: cd PATHNAME description: Sets PATHNAME as current working directory");
                printf("\n what it does: history descriint pid = bg_arr[xption: Prints list of previously executed commands(Max 10)\ncommand: kill PID description: Terminates process identified locally with PID in jobs list");
                printf("\n what it does : kill without background jobs will close the terminal");
                printf("\n what it does: !CMD desbuilt_in_cmd_flag=1;cription: runs the command numbered CMD in command history\ncommand: exit description: Terminates shell if and only if there are no background jobs\ncommand: help description: prints list of built in commands along with their description\n");
                internal_cmd_flag=1;
            }

            // help copied from pdf 
            if (strncmp(input,"cd",2)==0){
            
                if (chdir(cmd->CommArray->VarList[1])==-1){
                    printf("Are you sure that path exists ?");
                }
                internal_cmd_flag=1;

            }
            if (strncmp(input,"kill",4)==0){
            
                remove_at_kill(input);
                internal_cmd_flag=1;
            }
            if (strncmp(input,"!cmd",4)==0){
                printf("%s",&input[4]);
                int index = atoi (&input[4]) ;
                parseInfo* new =parse(hist_arr[index]);
                execvp(new->CommArray->VarList[0],new->CommArray->VarList);
                internal_cmd_flag=1;
            }
            if (strncmp(input,"history", 7) ==0 ){
                for (int i=1; i<hist_util;i++){
                    printf("[%d] %s \n",i, hist_arr[i] );
                    internal_cmd_flag=1;

                }
            }
            if (strncmp(input,"exit",4)==0){
                if (bg_util>0){
                    printf("there are jobs running");
                }
                else{
                break;}

            }        
            if (strncmp(input,"jobs", 4) ==0 ){
                printf("Number  ProcessID  Name \n");
                for (int i =0; i<bg_util;i++){
                    printf("[%d]    [%d]        %s \n",i+1, bg_arr[i].pid, bg_arr[i].name );
                }
                internal_cmd_flag=1;
            }
            int pid;
            if (empty_string==0){
                pid= fork();
            }
            if (pid==0){
                if (cmd->boolOutfile){
                    if (access(cmd->outFile,F_OK)==-1){
                        printf("dont have acesss to file");
                        return 1;
                    }
                    else{
                    int out = open(cmd->outFile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR |O_APPEND);
                    dup2(out,1);
                        }
                }
                if (cmd->boolInfile){
                    if (access(cmd->inFile,F_OK)==-1){
                    printf("file does not exist or has permission issues");
                        return 1;
                    }
                    else{
                        int in = open(cmd->inFile, O_RDONLY);
                        dup2(in,0);
                        
                    }
                }
                if (internal_cmd_flag==0){
                    if (execvp(cmd->CommArray->VarList[0],cmd->CommArray->VarList)==-1){
                        printf(" Please input a valid command");
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

            }
            if (pid < 0){
                continue;
            }
            }
            return 1;}