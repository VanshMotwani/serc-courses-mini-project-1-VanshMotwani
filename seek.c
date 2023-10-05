#include "headers.h"

void seek(char** correct_Paths, int* arrlen, char* target_path, char* to_find, int onlyFile, int onlyDir, int eflag, int* presenceOfFile, int* presenceOfDir, char** color_arr){
    DIR *dir = opendir(target_path);
    if (!dir) {
        printf("Failed to open directory: %s\n", target_path);
        return;
    }

    char** constrings= (char**)malloc(10000*sizeof(char*));
    int counting=0;

    struct dirent *entry;
    
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        // if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", target_path, entry->d_name);
        
        struct stat file_stat;
        if (stat(full_path, &file_stat) == -1) {
            printf("Cannot get information of file: %s\n", full_path);
            continue;
        }

        char filetype[100];
        if (S_ISREG(file_stat.st_mode)) {
            strcpy(filetype, "Regular File");
        } 
        else if (S_ISDIR(file_stat.st_mode)) {
            strcpy(filetype ,"Directory");
        } 
        else if (S_ISLNK(file_stat.st_mode)) {
            strcpy(filetype, "Symbolic Link");
        } 
        else {
            strcpy(filetype,"Other");
        }
        
        char *token;
        // Get the first token
        token = strtok(entry->d_name, "/");
        char filename[500];
        // Continue to get the remaining tokens
        while (token != NULL) {
            strcpy(filename,token);
            token = strtok(NULL, "/");
        }

        // printf("%s %s\n", entry->d_name, filename);
        // printf("full_path: %s\n", full_path);

        if (strcmp(to_find, filename) == 0){
            if ((strcmp(filetype, "Regular File") == 0) && (onlyFile || (!onlyDir))){
                strcpy(correct_Paths[*(arrlen)], full_path);
                color_arr[*arrlen]=(char*)malloc(10*sizeof(char));
                strcpy(color_arr[*arrlen],"file");
                *(arrlen)= *(arrlen)+1;
                *presenceOfFile=1;
            }
            if ((strcmp(filetype, "Directory") == 0) && (onlyDir || (!onlyFile))){
                strcpy(correct_Paths[*(arrlen)], full_path);
                color_arr[*arrlen]=(char*)malloc(10*sizeof(char));
                strcpy(color_arr[*arrlen],"dir");
                *(arrlen)= *(arrlen)+1;
                *presenceOfDir=1;
            }
        }

        if (strcmp(filetype, "Directory") == 0){
            seek(correct_Paths, arrlen, full_path, to_find, onlyFile, onlyDir, eflag, presenceOfFile, presenceOfDir, color_arr);
        }

    }

    closedir(dir);   
}

void seek_file(char** command_line_args, int cnt, int* presenceOfFile, int* presenceOfDir){
    *presenceOfDir=0;
    *presenceOfFile=0;

    char** color_arr= (char**)malloc(5000*sizeof(char*));

    char** path_for_seek= (char**)malloc(10000*sizeof(char*)); // Questionable
    for (int i=0; i<10000; i++){
        path_for_seek[i]=(char*)malloc(4096*sizeof(char));
    }

    int* seekarrlen= (int*)malloc(sizeof(int));
    *seekarrlen=0;

    char* to_find= (char*)malloc(4096*sizeof(char));
    char target_directory[4096];
    
    int onlyFiles=0, onlyDir=0, eflag=0;
    int failedOP=0;

    for (int i=1; i<cnt; i++){
        if (strcmp(command_line_args[i], "-d") == 0){
            onlyDir=1;
        }
        else if (strcmp(command_line_args[i], "-f") == 0){
            onlyFiles=1;
        }
        else if (strcmp(command_line_args[i], "-e") == 0){
            eflag=1;
        }
        else if (strcmp(command_line_args[i], "-de") == 0 || strcmp(command_line_args[i], "-ed") == 0){
            onlyDir=1;
            eflag=1;
        }
        else if (strcmp(command_line_args[i], "-fe") == 0 || strcmp(command_line_args[i], "-ef") == 0){
            onlyFiles=1;
            eflag=1;
        }
        else if (strcmp(command_line_args[i], "-df") == 0 || strcmp(command_line_args[i], "-fd") == 0){
            onlyFiles=1;
            onlyDir=1;
        }
        else if (i == (cnt-2)){
            strcpy(to_find, command_line_args[cnt-2]);
            strcpy(target_directory, command_line_args[cnt-1]);
            break;
        }
        else if (i == (cnt-1)){
            strcpy(to_find, command_line_args[cnt-1]);
            getcwd(target_directory, sizeof(target_directory));
            break;
        }
        else{
            printf("Invalid arguement %s for function \"seek\"\n", command_line_args[i]);
            failedOP=1;
            break;
        }
    }

    if (failedOP){
        return;
    }

    if (onlyDir && onlyFiles){
        printf("Invalid Flags!\n");
        return;
    }

    seek(path_for_seek, seekarrlen, target_directory, to_find, onlyFiles, onlyDir, eflag, presenceOfFile, presenceOfDir, color_arr);

    if (*(seekarrlen) == 1 && eflag){
        if (presenceOfDir){
            chdir(path_for_seek[0]);
            printf("\033[1;34m%s\033[0m\n", path_for_seek[0]);

        }
        else{
            for (int i=0; i<*(seekarrlen); i++){
                strcpy(path_for_seek[i], strstr(path_for_seek[i], target_directory));
                strcpy(path_for_seek[i], &(path_for_seek[i][strlen(target_directory)-1]));
                path_for_seek[i][0]='.';
                printf("\033[1;32m%s\033[0m\n", path_for_seek[i]);
            }
        }
    } 
    else{
        for (int i=0; i<*(seekarrlen); i++){
            strcpy(path_for_seek[i], strstr(path_for_seek[i], target_directory));
            strcpy(path_for_seek[i], &(path_for_seek[i][strlen(target_directory)-1]));
            path_for_seek[i][0]='.';
            if (strcmp(color_arr[i], "file") == 0){
                printf("\033[1;32m%s\033[0m\n", path_for_seek[i]);
            }
            else{
                printf("\033[1;34m%s\033[0m\n", path_for_seek[i]);
            }
        }
    }
}