#ifndef __EXEC_CMD_H
#define __EXEC_CMD_H

void identify_and_execute(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int bg, char*** background_processes_array, char** foreground_processes_array, int* num_bg_proccesses, int* num_bg_proc_args);

#endif