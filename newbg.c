#include "headers.h"

void runBackgroundProcesses(char **foreground_args, char ***background_args, int* num_background_processes, int *background_arg_counts) {
    if (strcmp(foreground_args[0], "") == 0){
        return;
    }

    // Array to store child PIDs of background processes
    pid_t background_child_pids[*num_background_processes];

    // Fork a child process for foreground
    pid_t foreground_child_pid = fork();

    if (foreground_child_pid == 0) {
        // Child process for foreground
        execvp(foreground_args[0], foreground_args);

        perror("execvp failed for foreground process"); // Print an error message if execvp fails
        exit(1);
    } 
    else if (foreground_child_pid > 0) {
        // Parent process
        // Fork multiple child processes for background
        for (int i = 0; i < *num_background_processes; i++) {
            background_child_pids[i] = fork();

            if (background_child_pids[i] == 0) {
                // Child process for background
                execvp(background_args[i][0], background_args[i]);
                perror("execvp for background"); // Print an error message if execvp fails
                exit(1);
            } else if (background_child_pids[i] < 0) {
                perror("fork for background"); // Print an error message if fork fails
                exit(1);
            }
            // Else, continue to fork next background process
        }

        // Save the background child process PIDs for later reference
        // printf("Background processes started with PIDs: ");
        for (int i = 0; i < *num_background_processes; i++) {
            printf("%d\n", background_child_pids[i]);
        }

        // Wait for the foreground process to complete
        waitpid(foreground_child_pid, NULL, 0);

        // Optionally, periodically check the /proc/stat or use waitpid with WNOHANG for the background processes

        // Proceed with other tasks
    } 
    else {
        perror("fork for foreground"); // Print an error message if fork fails
        exit(1);
    }

    // Check if the background processes have completed execution
    // for (int i = 0; i < *num_background_processes; i++) {
    //     printf("Background process %d status: %d\n", i, background_child_pids[i]);
    //     // You can add code here to check the status of each background process
    // }
}

    // for (int i=0; i<*num_background_processes; i++){
    //     printf("BG Proccess %d:\n", i);
    //     for (int j=0; j<background_arg_counts[i]; j++){
    //         printf("%s ", background_args[i][j]);
    //     }
    //     printf("\n");
    // }    

    // int i = 0;
    // char* k;
    // while (k != NULL){
    //     k= foreground_args[i];
    //     printf("%s\n", k);
    //     i++;
    // }