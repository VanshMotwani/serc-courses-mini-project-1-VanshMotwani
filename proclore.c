#include "headers.h"

void proclore(char* pid){
    char path_to_fdata[4096];
    snprintf(path_to_fdata, sizeof(path_to_fdata), "/proc/%s/stat", pid);

    char path_to_exe[4096];
    snprintf(path_to_exe, sizeof(path_to_exe), "/proc/%s/exe", pid);
    
    char executable_path[4096];
    ssize_t exe_path_len = readlink(path_to_exe, executable_path, sizeof(executable_path) - 1);
    if (exe_path_len == -1) {
        printf("Failed to read executable path for %s\n", path_to_exe);
    } 
    else {
        executable_path[exe_path_len] = '\0';
    }


    FILE *stat_file = fopen(path_to_fdata, "r");
    if (!stat_file) {
        printf("Failed to open stat file");
        return;
    }

    char process_status;
    fscanf(stat_file, "%*d %*s %c", &process_status);

    char line_by_line[4096];
    long virtual_memory;
    while (fgets(line_by_line, sizeof(line_by_line), stat_file)) {
        if (strncmp(line_by_line, "VmSize:", 7) == 0) {
            if (sscanf(line_by_line, "VmSize:%ld", &virtual_memory) != 1) {
                printf("Virtual memory cannot be accesed for process %s\n", pid);
            }
            break; 
        }
    }

    pid_t group_id;
    fscanf(stat_file, "%*d %*s %*c %d", &group_id);

    fclose(stat_file);

    printf("pid : %s\n", pid);
    if (process_status == 'R' || process_status == 'S'){
        printf("process status : %c+\n", process_status);
    }
    else{
        printf("process status : %c\n", process_status);
    }
    
    printf("Process Group : %d\n", group_id);

    printf("Virtual memory : %ld\n", virtual_memory);

    printf("Executable path : %s\n\n", executable_path);

}