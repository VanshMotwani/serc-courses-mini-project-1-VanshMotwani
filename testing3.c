#include "processes.h"

int main() {
    char input[4096];
    fgets(input, 4096, stdin);

    input[strlen(input)-1]= '\0';
    handleInput(input);

    // char *foreground_args[] = {"sleep", "5", NULL};

    // char *background_args[] = {"sleep", "10", NULL};

    // char *background_args_2[] = {"echo", "15", NULL};

    // pid_t proc_pid= background_exec(background_args);
    // // pid_t proc_pid_2= background_exec(background_args_2);
    
    // foreground_exec(foreground_args);
    // foreground_exec(background_args_2);

    // printf("%d\n", proc_pid);
}