#include "headers.h"

int compare_strings(const void *a, const void *b) {
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2);
}

void peek(char* path, int a, int l, char* home_dir, char* prev_dir){
    if (strcmp(path, "-") == 0){
        strcpy(path, prev_dir);
    }
    if (path[0] == '~'){
        strcpy(path, &(path[1]));
        char newpath[4096];
        strcpy(newpath, home_dir);
        strcat(newpath, path);
        strcpy(path, newpath);
    }

    DIR *dir = opendir(path);
    if (!dir) {
        printf("Failed to open directory: %s\n", path);
        return;
    }

    char** constrings= (char**)malloc(10000*sizeof(char*));
    int counting=0;

    struct dirent *entry;
    
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.'){
            if (!a){
                continue;
            }
        }

    // if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
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
        
        char permissions[14] = "--- --- --- -";
        if (file_stat.st_mode & S_IRUSR) permissions[0] = 'r';
        if (file_stat.st_mode & S_IWUSR) permissions[1] = 'w';
        if (file_stat.st_mode & S_IXUSR) permissions[2] = 'x';
        if (file_stat.st_mode & S_IRGRP) permissions[4] = 'r';
        if (file_stat.st_mode & S_IWGRP) permissions[5] = 'w';
        if (file_stat.st_mode & S_IXGRP) permissions[6] = 'x';
        if (file_stat.st_mode & S_IROTH) permissions[8] = 'r';
        if (file_stat.st_mode & S_IWOTH) permissions[9] = 'w';
        if (file_stat.st_mode & S_IXOTH) permissions[10] = 'x';

        char ownername[500];
        struct passwd *owner_info = getpwuid(file_stat.st_uid);
        if (owner_info) {
            strcpy(ownername ,owner_info->pw_name);
        }

        char groupname[500];
        struct group *group_info = getgrgid(file_stat.st_gid);
        if (group_info) {
            strcpy(groupname, group_info->gr_name);
        }

        struct tm *mod_time = localtime(&file_stat.st_mtime);
        char time_buffer[80];
        strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", mod_time);

        char *token;
        // Get the first token
        token = strtok(entry->d_name, "/");
        char filename[500];
        // Continue to get the remaining tokens
        while (token != NULL) {
            strcpy(filename,token);
            token = strtok(NULL, "/");
        }

        char buffer[10000];// = (char*)malloc(10000*sizeof(char));
        snprintf(buffer, sizeof(buffer), "%s %s;%s;%ld;%s;%s;%ld;%s\n", filename, filetype, permissions, file_stat.st_nlink, ownername, groupname, file_stat.st_size, time_buffer);
        //printf("%-20s %-20s %-15s %-10ld %-10s %-10s %-20ld %-25s\n", filename, filetype, permissions, file_stat.st_nlink, ownername, groupname, file_stat.st_size, time_buffer);

        constrings[counting]= (char*)malloc(10000*sizeof(char));
        strcpy(constrings[counting], buffer);
        counting++;
    }

    // qsort(constrings, counting, 10000, compare_strings);
    qsort(constrings, counting, sizeof(char *), compare_strings);
    
    char* filename= (char*)malloc(500*sizeof(char));
    
    if (!l){
        printf("%-20s\n", "File Name");
        printf("--------------------\n");

        // for (int i=0; i<counting; i++){
        //     filename= strtok(constrings[i], " ");    
        //     printf("%-20s\n", filename);
        // }

        char* filetype= (char*)malloc(100*sizeof(char));

        for (int i=0; i<counting; i++){
            filename= strtok(constrings[i], " ");
            filetype= strtok(NULL, ";");
            if (strcmp(filetype, "Regular File") == 0){
                printf("\033[0m%-20s\n", filename); // White color for regular file
            }
            else if (strcmp(filetype, "Directory") == 0){
                printf("\033[1;34m%-20s\033[0m\n", filename); // Blue for directories
            }
            else{
                printf("\033[1;32m%-20s\033[0m\n", filename); // Green color for executables
            }
        }
    }
    else{
        printf("%-20s %-20s %-15s %-10s %-10s %-10s %-20s %-25s\n", "File Name", "File Type", "Permissions", "Hard Links", "Owner", "Group", "Size(bytes)", "Last Modified");
        printf("-------------------- -------------------- --------------- ---------- ---------- ---------- -------------------- -------------------------\n");

        char* filetype= (char*)malloc(100*sizeof(char));
        char* permissions= (char*)malloc(14*sizeof(char)); 
        char* hardlinks= (char*)malloc(500*sizeof(char));
        char* ownername= (char*)malloc(500*sizeof(char));
        char* groupname= (char*)malloc(500*sizeof(char));
        char* sizebytes= (char*)malloc(500*sizeof(char));
        char* timebuffer= (char*)malloc(500*sizeof(char));

        for (int i=0; i<counting; i++){
            filename= strtok(constrings[i], " ");
            filetype= strtok(NULL, ";");
            permissions= strtok(NULL, ";");
            hardlinks= strtok(NULL, ";");
            ownername= strtok(NULL, ";");
            groupname= strtok(NULL, ";");
            sizebytes= strtok(NULL, ";");
            timebuffer= strtok(NULL, ";");
            if (strcmp(filetype, "Regular File") == 0){
                // White color for regular file
                printf("\033[0m%-20s %-20s %-15s %-10s %-10s %-10s %-20s %-25s\n", filename, filetype, permissions, hardlinks, ownername, groupname, sizebytes, timebuffer);

            }
            else if (strcmp(filetype, "Directory") == 0){
                // Blue for directories
                printf("\033[1;34m%-20s\033[0m %-20s %-15s %-10s %-10s %-10s %-20s %-25s\n", filename, filetype, permissions, hardlinks, ownername, groupname, sizebytes, timebuffer);
                
            }
            else{
                // Green color for executables
                printf("\033[1;32m%-20s\033[0m %-20s %-15s %-10s %-10s %-10s %-20s %-25s\n", filename, filetype, permissions, hardlinks, ownername, groupname, sizebytes, timebuffer);

            }
        }
    }

    // for (int i = 0; i < counting; i++) {
    //     sscanf(constrings[i], "%s %s;%s;%ld;%s;%s;%ld;%s", filename, filetype, permissions, hardlinks, ownername, groupname, sizebytes, timebuffer);

    //     printf("%-20s %-20s %-15s %-10ld %-10s %-10s %-20ld %-25s\n", filename, filetype, permissions, hardlinks, ownername, groupname, sizebytes, timebuffer);
    //     // printf("%s\n", constrings[i]);
    // }
    
    // int lenArr[counting];
    // for (int i=0; i<counting; i++){
    //     lenArr[i]= strlen(constrings[i]);
    // }
    // quickSort_for_Strings(constrings, lenArr, 0, counting);
    

    closedir(dir);   
}