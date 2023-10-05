#ifndef __PROCESSING_H
#define __PROCESSING_H

void runInFG(char*** cmd, int pipeCount, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc);

void runInBG(char*** cmd, int pipeCount, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc);

void execute_bg(char* pid);

void execute_fg(char* pid);

void ping(char* signal, char* pid_input);

#endif
