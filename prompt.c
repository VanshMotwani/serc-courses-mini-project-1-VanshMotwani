#include "headers.h"

void prompt(char* home_dir) {
    // Do not hardcode the prmopt
    char directory_path[4096];  // A buffer to hold the directory path
    char username[4096];
    char systemname[4096];

    if (getlogin_r(username, sizeof(username)) != 0){
        printf("User name not found\n");
    }

    if (gethostname(systemname, sizeof(systemname)) != 0){
        printf("System name not found\n");
    }

    if (getcwd(directory_path, sizeof(directory_path)) == NULL) {
        printf("Current directory path not found\n");
    }

    // printf("%s\n", home_dir);
    // printf("%s\n", directory_path);

    char tempChar= 'a';
    int counter=0;
    int is_home=1;
    while (counter < strlen(home_dir)){
        tempChar=directory_path[counter];
       // printf("%c/%c ", tempChar, directory_path[counter]);
        if (tempChar != home_dir[counter]){
            is_home=0;
            break;
        }
        counter++;
    }

    if (is_home){
        counter--;
        directory_path[counter]='~';
        strcpy(directory_path,&(directory_path[counter]));
    }

    printf("%s@%s:%s> ", username, systemname, directory_path);
    // printf("<Everything is a file> ");    
}