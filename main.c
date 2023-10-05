#include "headers.h"

void ctrl_z_handler(int signum) {
    pid_t shell_pgid = getpgrp();

    // Check if the shell is in the foreground
    if (tcgetpgrp(STDIN_FILENO) == shell_pgid) {
        // Get the PID of the foreground process
        pid_t foreground_pid = tcgetpgrp(STDIN_FILENO);

        // Check if there is a foreground process running
        if (foreground_pid > 0) {
            // Send the SIGSTOP signal to the foreground process
            if (kill(foreground_pid, SIGSTOP) == 0) {
                printf("Process with PID %d has been stopped.\n", foreground_pid);
            } else {
                perror("Error sending SIGSTOP");
            }
        } else {
            printf("No foreground process is running.\n");
        }
    } else {
        printf("The shell is not in the foreground.\n");
    }
}

int main()
{
    int *BG_PIDs = (int *)malloc(100 * sizeof(int)); // Allocate an array of 100 integers

    int *num_bg_proc = (int *)malloc(sizeof(int)); // Allocate an integer pointer

    // Initialize num_bg_proc or set it to an initial value
    *num_bg_proc = 0;

    signal(SIGTSTP, ctrl_z_handler);
    // Keep accepting commands
    char home_dir[4096];  // A buffer to hold the directory path

    if (getcwd(home_dir, sizeof(home_dir)) == NULL) {
        printf("Current directory path not found\n");
    }
    
    int* isFirstTime=(int*)malloc(sizeof(int));
    *(isFirstTime)=1;
    char prev_dir_1[4096];
    char prev_dir_2[4096];

    pid_t my_pid = getpid();
    // Convert the PID to a string 
    char shell_PID[100]; 
    snprintf(shell_PID, sizeof(shell_PID), "%d", my_pid);

    FILE *myFile = fopen("pastevents.txt", "a+"); // Open file for both reading and appending
    if (!myFile) {
        printf("Error opening pastevents.txt\n");
    }

    char prevInput[4096]= "";

    char** pasteventsarr= (char**)malloc(15*sizeof(char*)); // Questionable
    for (int i=0; i<15; i++){
        pasteventsarr[i]=(char*)malloc(4096*sizeof(char));
    }


    int* arrlen= (int*)malloc(sizeof(int));               // Questionable
    *(arrlen)=0;

    int* start= (int*)malloc(sizeof(int));
    int* cur= (int*)malloc(sizeof(int));
    *(start)=0;
    *(cur)=0;

    // MAYBE POPULATE THE ARRAY WITH THE FILE CONTENTS?
    populate_array_from_file(pasteventsarr, arrlen, start, cur);

    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        prompt(home_dir);
        char input_b4[4096];
        fgets(input_b4, 4096, stdin);
        input_b4[strlen(input_b4)-1]='\0';

        if (input_b4[0] == '\0'){
            continue;
        }

        char* input= (char*)malloc(4096*sizeof(char));
        strcpy(input, input_b4);

        int pasteventsPresenceFlag=0;
        char copy_input[4096];
        strcpy(copy_input, input);
        char* copy_token= (char*)malloc(4096*sizeof(char));
        copy_token = strtok(copy_input, " ");

        if (strcmp(copy_token, "pastevents") == 0)
            pasteventsPresenceFlag=1;
       
        while(1){
            copy_token = strtok(NULL, " ");
            if (copy_token == NULL)
                break; 
            if (strcmp(copy_token, "pastevents") == 0)
                pasteventsPresenceFlag=1;
        }
        
        if (strcmp(input, "\0") != 0 && strcmp(input, "") != 0){
            if (strcmp(input, prevInput) != 0){
                if (!pasteventsPresenceFlag){    // not pasteventsflag
                    add_in_array(pasteventsarr, input, arrlen, start, cur);
                    strcpy(prevInput, input);
                }
            }
        }

        char* newinput= (char*)malloc(4096*sizeof(char));
        int* isPastExecPresent= (int*)malloc(sizeof(int));
        *isPastExecPresent=0;

        add_to_pastevents_arr(input, pasteventsarr, prevInput, arrlen, start, cur, newinput, isPastExecPresent);

        // printf("%d\n", *(isPastExecPresent));
        if (*(isPastExecPresent)){
            strcpy(input, newinput);
        }
        
        // int bg=1;
        // identify_and_execute(input, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, bg);
        handleInput(input, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);

        // for (int l=0; l<*num_bg_proc; l++){
        //     int pid= BG_PIDs[l];

        // }
    }
}

        // char* token= (char*)malloc(4096*sizeof(char));
        // char** command_line_args= (char**)malloc(100*sizeof(char*));
        // command_line_args[0]=(char*)malloc(4096*sizeof(char));
        // token = strtok(input, " ");
        // strcpy(command_line_args[0], token);
        // int cnt=1;
        // if (*(isFirstTime)){
        //     strcpy(prev_dir_2,home_dir);
        //     strcpy(prev_dir_1, "\0");
        //     *(isFirstTime)=0;
        // }

        // while(1){
        //     token = strtok(NULL, " ");
        //     if (token == NULL){
        //         break;
        //     }   
        //     command_line_args[cnt]=(char*)malloc(4096*sizeof(char));
        //     strcpy(command_line_args[cnt], token);
        //     cnt++;
        // }

        // if (strcmp(command_line_args[0], "warp") == 0){
        //     warp_execute(command_line_args, cnt, home_dir, prev_dir_1, prev_dir_2);
        // }
        // else if (strcmp(command_line_args[0], "peek") == 0){
        //     int aflag=0;
        //     int lflag=0;
        //     int isitpath[cnt];
        //     for (int i=0; i<cnt; i++){
        //         isitpath[i]=1;
        //     }
            
        //     char cur_direct[4096];
        //     for (int i=1; i<cnt; i++){
        //         if (strcmp(command_line_args[i], "-a")==0){
        //             aflag=1;
        //             isitpath[i]=0;
        //         }
        //         if (strcmp(command_line_args[i], "-al")==0 || strcmp(command_line_args[i], "-la")==0){
        //             aflag=1;
        //             lflag=1;
        //             isitpath[i]=0;
        //         }
        //         if (strcmp(command_line_args[i], "-l")==0){
        //             lflag=1;
        //             isitpath[i]=0;
        //         }
        //     }
        //     int pathindex=0;
        //     for (int i=1; i<cnt; i++){
        //         if (isitpath[i]){
        //             pathindex=i;
        //             break;
        //         }
        //     }
        //     if (pathindex==0){
        //         getcwd(cur_direct, 4096);
        //         peek(cur_direct, aflag, lflag, home_dir, prev_dir_1);
        //     }
        //     else{
        //         peek(command_line_args[pathindex], aflag, lflag, home_dir, prev_dir_1);
        //     }          
        // }
        // else if (strcmp(command_line_args[0], "proclore") == 0){
        //     if (cnt == 1){
        //         proclore(shell_PID);
        //     }
        //     else{
        //         proclore(command_line_args[1]);
        //     }
        // }
        // else if (strcmp(command_line_args[0], "pastevents") == 0){
        //     if (cnt == 1){
        //         // pastevents
        //         for (int i=*(start); i!=*(cur); i=(i+1)%15){
        //             printf("%d:%s\n", i, pasteventsarr[i]);
        //         }
        //         printf("%d:%s\n",*(cur), pasteventsarr[*(cur)]);
        //     }
        //     else if (strcmp(command_line_args[1], "purge") == 0){
        //         // pastevents purge
        //         FILE* pastFile = fopen("pastevents.txt", "w");
        //         if (!pastFile) {
        //             printf("Error opening pastevents.txt\n");
        //         }
        //         for (int i=0; i<15; i++){
        //             free(pasteventsarr[i]);
        //         }
        //         for (int i=0; i<15; i++){
        //             pasteventsarr[i]=(char*)malloc(4096*sizeof(char));
        //         }
        //         *(arrlen)=0; *(start)=0; *(cur)=1;
        //     }
        //     else if (strcmp(command_line_args[1], "execute") == 0){
        //         // pastevents execute
                
        //     }
        // }
        // else if (strcmp(command_line_args[0], "seek") == 0){
        //     seek_file(command_line_args, cnt);
        // }