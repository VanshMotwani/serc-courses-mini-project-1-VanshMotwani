#include "headers.h"

void background_exec(char** background_args) {
    int child_pid = fork();  // Create a child process

    if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Child process
        printf("Child process: PID=%d\n", getpid());

        // char *args[] = {"sleep", "5", NULL};  // Command and its arguments
        execvp(background_args[0], background_args);  // Replace child process with 'sleep' command
        printf("execvp failed\n");  // This line will only execute if execvp fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process

        // No need to wait for the child to finish
    }

    return;
}