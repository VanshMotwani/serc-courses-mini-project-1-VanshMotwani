#include "headers.h"

void runInBG(char*** cmd, int pipeCount, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc){
    signal(SIGTSTP, ctrl_z_handler);
    if (strcmp(cmd[0][0], "") == 0){
        return;
    }

    // Form something to store the PIDs of BG and FG processes

    pid_t cur_id= fork();

    if (cur_id == 0){
        runSortedPipedCommands(pipeCount, cmd, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
        // perror(cmd[0][0]);
        exit(0);
    }
    else if (cur_id > 0){
        BG_PIDs[*num_bg_proc]=cur_id;
        *num_bg_proc= (*num_bg_proc + 1)%100;
        // waitpid(cur_id, NULL, 0);
        // Do nothing
    }
    else{
        perror("Error in forking");
        exit(0);
    }

}

void runInFG(char*** cmd, int pipeCount, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc){
    signal(SIGTSTP, ctrl_z_handler);
    if (strcmp(cmd[0][0], "") == 0){
        return;
    }

    // Form something to store the PIDs of BG and FG processes

    pid_t cur_id= fork();

    if (cur_id == 0){
        runSortedPipedCommands(pipeCount, cmd, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
        // perror(cmd[0][0]);
        exit(0);
    }
    else if (cur_id > 0){
        waitpid(cur_id, NULL, 0);
    }
    else{
        perror("Error in forking");
        exit(0);
    }

}

void execute_fg(char* pid)
{
    int status;
    waitpid(atoi(pid), &status, WUNTRACED);
    tcsetpgrp(STDIN_FILENO, atoi(pid));
    kill(atoi(pid), SIGCONT);
    waitpid(atoi(pid), &status, WCONTINUED);
    tcsetpgrp(STDIN_FILENO, getpgrp());
}

void execute_bg(char* pid)
{
    char signal[100];
    sprintf(signal, "%d", SIGCONT);

    ping(pid, signal);
}

void ping(char* signal, char* pid_input) {
    pid_t pid = atoi(pid_input);
    int signal_number = atoi(signal);

    // if (getpgid(pid) < 0) {
    //     perror("No such process found");
    //     return;
    // }

    signal_number %= 32;

    if (kill(pid, signal_number) == 0){
        return;
    } 
    else{
        perror("Error sending signal");
        return;
    }

    return;
}