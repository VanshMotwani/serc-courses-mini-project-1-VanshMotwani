#ifndef __PIPING_H
#define __PIPING_H

void handleIORedirection(char** input);

void runPipedCommands(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc);

void sortPipedCommands(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int bg, int* BG_PIDs, int* num_bg_proc);

void runSortedPipedCommands(int pipeCount, char*** cmd, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc);

#endif