#include "headers.h"

void foreground_exec(char** foreground_args) {
    pid_t foreground_child_pid;

    foreground_child_pid = fork();

    if (foreground_child_pid == 0) {
        execvp(foreground_args[0], foreground_args);
        printf("Invalid command/ arguements for %s\n", foreground_args[0]);
        exit(1);
    } else if (foreground_child_pid > 0) {
        waitpid(foreground_child_pid, NULL, 0);
    } else {
        perror("fork for foreground");
        exit(1);
    }

    // Check if the foreground process has completed execution

    return;
}