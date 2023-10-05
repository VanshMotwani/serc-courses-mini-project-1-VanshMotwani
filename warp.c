#include "headers.h"

void warp(char* input, char* home_dir, char* prev_dir){

    if (input[0] == '/'){
        
        if (chdir(input) != 0){
            printf("Error in changing directory to %s\n", input);
        }
        
        printf("%s\n", input);
    }
    else if (input[0] == '.' && input[1] == '/'){
        if (chdir(input) != 0){
            printf("Error in changing directory to %s\n", input);
        }

        char cur_dir[4096];  // A buffer to hold the directory path

        if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
            printf("Current directory path not found\n");
        }

        printf("%s\n", cur_dir);
    }
    else if (input[0] == '.' && input[1] == '.' && input[2] == '/'){
        if (chdir(input) != 0){
            printf("Error in changing directory to %s\n", input);
        }

        char cur_dir[4096];  // A buffer to hold the directory path

        if (getcwd(cur_dir, sizeof(cur_dir)) == NULL) {
            printf("Current directory path not found\n");
        }
        
        printf("%s\n", cur_dir);
    }
    else if (input[0] == '~'){
        char newinput[4096];
        strcpy(input, &(input[1]));
        strcpy(newinput, home_dir);
        strcat(newinput, input);
        printf("%s\n", newinput);

        if (chdir(newinput) != 0){
            printf("Error in changing directory to %s\n", newinput);
        }
    }
    else if (input[0] == '-'){
        if (strcmp(prev_dir, "\0") == 0){
            printf("The current directory is the first directory opened after starting shell.\n            Previous directory does not exist.\n");
        }
        else if (chdir(prev_dir) != 0){
            printf("Error in changing directory to %s\n", prev_dir);
        }
    }
}

void warp_execute(char** command_line_args, int cnt, char* home_dir, char* prev_dir_1, char* prev_dir_2){
    if (cnt == 1){
        warp(home_dir, home_dir, prev_dir_1);
        strcpy(prev_dir_1,prev_dir_2);
        strcpy(prev_dir_2, home_dir);
    }
    else{
        for (int i=1; i<cnt; i++){
            warp(command_line_args[i], home_dir, prev_dir_1);
            strcpy(prev_dir_1, prev_dir_2);
            getcwd(prev_dir_2, sizeof(prev_dir_2));
        }
    }
}