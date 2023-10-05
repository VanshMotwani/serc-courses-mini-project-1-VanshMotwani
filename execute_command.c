#include "headers.h"

void identify_and_execute(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int bg, char*** background_processes_array, char** foreground_processes_array, int* num_bg_proccesses, int* num_bg_proc_args){
    char* token= (char*)malloc(4096*sizeof(char));
    char** command_line_args= (char**)malloc(100*sizeof(char*));
    command_line_args[0]=(char*)malloc(4096*sizeof(char));
    char* input_cpy= (char*)malloc(4096*sizeof(char));
    strcpy(input_cpy, input);

    token = strtok(input_cpy, " ");
    strcpy(command_line_args[0], token);
    int cnt=1;
    if (*(isFirstTime)){
        strcpy(prev_dir_2,home_dir);
        strcpy(prev_dir_1, "\0");
        *(isFirstTime)=0;
    }

    int* presenceOfFile= (int*)malloc(sizeof(int));
    int* presenceOfDir= (int*)malloc(sizeof(int));

    while(1){
        token = strtok(NULL, " ");
        if (token == NULL){
            break;
        }   
        command_line_args[cnt]=(char*)malloc(4096*sizeof(char));
        strcpy(command_line_args[cnt], token);
        cnt++;
    }

    if (strcmp(command_line_args[0], "warp") == 0){
        warp_execute(command_line_args, cnt, home_dir, prev_dir_1, prev_dir_2);
    }
    else if (strcmp(command_line_args[0], "peek") == 0){
        int aflag=0;
        int lflag=0;
        int isitpath[cnt];
        for (int i=0; i<cnt; i++){
            isitpath[i]=1;
        }
        
        char cur_direct[4096];
        for (int i=1; i<cnt; i++){
            if (strcmp(command_line_args[i], "-a")==0){
                aflag=1;
                isitpath[i]=0;
            }
            if (strcmp(command_line_args[i], "-al")==0 || strcmp(command_line_args[i], "-la")==0){
                aflag=1;
                lflag=1;
                isitpath[i]=0;
            }
            if (strcmp(command_line_args[i], "-l")==0){
                lflag=1;
                isitpath[i]=0;
            }
        }
        int pathindex=0;
        for (int i=1; i<cnt; i++){
            if (isitpath[i]){
                pathindex=i;
                break;
            }
        }
        if (pathindex==0){
            getcwd(cur_direct, 4096);
            peek(cur_direct, aflag, lflag, home_dir, prev_dir_1);
        }
        else{
            peek(command_line_args[pathindex], aflag, lflag, home_dir, prev_dir_1);
        }          
    }
    else if (strcmp(command_line_args[0], "proclore") == 0){
        if (cnt == 1){
            proclore(shell_PID);
        }
        else{
            proclore(command_line_args[1]);
        }
    }
    else if (strcmp(command_line_args[0], "pastevents") == 0){
        if (cnt == 1){
            // pastevents
            for (int i=*(start); i!=*(cur); i=(i+1)%15){
                printf("%s\n", pasteventsarr[i]);
            }
            printf("%s\n", pasteventsarr[*(cur)]);
        }
        else if (strcmp(command_line_args[1], "purge") == 0){
            // pastevents purge
            FILE* pastFile = fopen("pastevents.txt", "w");
            if (!pastFile) {
                printf("Error opening pastevents.txt\n");
            }
            for (int i=0; i<15; i++){
                free(pasteventsarr[i]);
            }
            for (int i=0; i<15; i++){
                pasteventsarr[i]=(char*)malloc(4096*sizeof(char));
            }
            *(arrlen)=0; *(start)=0; *(cur)=0;
        }
        else if (strcmp(command_line_args[1], "execute") == 0){
            // pastevents execute
            
        }
    }
    else if (strcmp(command_line_args[0], "seek") == 0){
        *presenceOfDir=0;
        *presenceOfFile=0;
        seek_file(command_line_args, cnt, presenceOfFile, presenceOfDir);
    }
    else{
        command_line_args[cnt]=NULL;
        
        if (bg) {
        // Store background command and arguments in background_processes_array
            for (int i = 0; i < cnt; i++) {
                background_processes_array[*num_bg_proccesses][i]= (char*)malloc(4096*sizeof(char));
                strcpy(background_processes_array[*num_bg_proccesses][i],command_line_args[i]);
            }

            background_processes_array[*num_bg_proccesses][cnt] = NULL;
            num_bg_proc_args[*num_bg_proccesses]=cnt;
            (*num_bg_proccesses)++;
        } 
        else {
            // Store foreground command and arguments in foreground_processes_array
            for (int i = 0; i < cnt; i++) {
                strcpy(foreground_processes_array[i],command_line_args[i]);
            }

            foreground_processes_array[cnt] = NULL;
        }
        
    }
}

    // for (int i = 0; i < *num_bg_proccesses; i++) {
    //     printf("Background Process %d:\n", i);
    //     for (int j = 0; j < num_bg_proc_args[i]; j++) {
    //         printf("  Arg[%d]: %s\n", j, background_processes_array[i][j]);
    //     }
    //     printf("  Arg Count: %d\n\n", num_bg_proc_args[i]);
    // }

    // // Print foreground_processes_array
    // printf("Foreground Process:\n");
    //     for (int i = 0; foreground_processes_array[i] != NULL; i++) {
    //         printf("  Arg[%d]: %s\n", i, foreground_processes_array[i]);
    // }

        // if (bg){
        //     int id= fork();

        //     if (id == 0){
        //         execvp(command_line_args[0], command_line_args);
        //     }
        //     else{
        //         // printf("Background process with id %d started\n", id);
        //     }
        // }
        // else{
        //     int foreid= fork();

        //     if (foreid == 0){
        //         execvp(command_line_args[0], command_line_args);
        //     }
        //     else{
        //         wait(NULL);
        //     }
        // }