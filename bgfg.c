#include "headers.h"

int main() {
    // Foreground process arguments
    char *foreground_args[] = {"invalid", "bitch", NULL};

    // Background process arguments
    char *background_args[] = {"sleep", "10", NULL};

    pid_t background_child_pid;
    // Fork a child process for foreground
    pid_t foreground_child_pid = fork();

    if (foreground_child_pid == 0) {
        // Child process for foreground
        execvp(foreground_args[0], foreground_args);
        perror("execvp for foreground"); // Print an error message if execvp fails
        exit(1);
    } else if (foreground_child_pid > 0) {
        // Parent process
        // Fork a child process for background
        background_child_pid = fork();

        if (background_child_pid == 0) {
            // Child process for background
            execvp(background_args[0], background_args);
            perror("execvp for background"); // Print an error message if execvp fails
            exit(1);
        } else if (background_child_pid > 0) {
            // Parent process
            // Save the background child process's PID for later reference
            printf("Background process started with PID: %d\n", background_child_pid);

            // Wait for the foreground process to complete
            waitpid(foreground_child_pid, NULL, 0);
            
            // Optionally, periodically check the /proc/stat or use waitpid with WNOHANG for the background process
            
            // Proceed with other tasks
        } else {
            perror("fork for background"); // Print an error message if fork fails
            exit(1);
        }
    } else {
        perror("fork for foreground"); // Print an error message if fork fails
        exit(1);
    }

    // Check if the background process has completed execution
    printf("%d\n", foreground_child_pid);

    char* PIDStr= (char*)malloc(4096*sizeof(char));
    sprintf(PIDStr, "%d", foreground_child_pid);

    char path_to_procdata[4096];
    snprintf(path_to_procdata, sizeof(path_to_procdata), "/proc/%s/stat", PIDStr);

    FILE *stat_file = fopen(path_to_procdata, "r");
    if (!stat_file) {
        printf("Failed to open stat file");
        return 0;
    }

    char process_status;
    fscanf(stat_file, "%*d %*s %c", &process_status);

    printf("process status:%c\n", process_status);

    return 0;
}