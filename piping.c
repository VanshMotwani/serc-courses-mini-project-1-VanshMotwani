#include "headers.h"

void handleIORedirection(char** input){
    // HANDLE Multiple I/O Redirections
    char* tempStr;
    int count = 0;
    int cmdEndIndex=-1;
    int lastOccurenceOfOutputRedir= -1;
    int lastOccurenceOfInputRedir= -1;
    while (1){
        tempStr= input[count];
        if (tempStr == NULL){
            break;
        }
        if (strcmp(tempStr,"<") == 0 || strcmp(tempStr,">>") == 0 || strcmp(tempStr,">") == 0){
            if (cmdEndIndex == -1){
                cmdEndIndex=count;
            }
            if (strcmp(tempStr,">") == 0 || strcmp(tempStr,">>") == 0){
                lastOccurenceOfOutputRedir=count;
            }
            if (strcmp(tempStr,"<") == 0){
                lastOccurenceOfInputRedir=count;
            }
        }
        count++;
    }
    
    if (cmdEndIndex == -1){
        return;
    }

    int IFILE; 
    int OFILE;
    
    if (lastOccurenceOfOutputRedir != -1){
        if (strcmp(input[lastOccurenceOfOutputRedir],">") == 0){
            OFILE= open(input[lastOccurenceOfOutputRedir+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);//S_IRWXG | S_IRWXO | S_IRWXU);
        }
        else if (strcmp(input[lastOccurenceOfOutputRedir],">>") == 0){
            OFILE= open(input[lastOccurenceOfOutputRedir+1], O_APPEND | O_CREAT, 0644); //S_IRWXG | S_IRWXO | S_IRWXU); 
        }

        if (OFILE == -1){
            perror("Error opening OFILE");
            return;
        }

        dup2(OFILE, STDOUT_FILENO);
        close(OFILE);
    }

    if (lastOccurenceOfInputRedir != -1){
        IFILE = open(input[lastOccurenceOfInputRedir+1], O_RDONLY); // "r" for read mode

        if (IFILE == -1) {            
            perror("Error opening IFILE");
            return; // Return an error code to indicate failure
        }

        dup2(IFILE, STDIN_FILENO);
        close(IFILE);
    }
  
    input[cmdEndIndex]=NULL;
    // execvp(input[0], input);
    // exit(0);
}

void runPipedCommands(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc){
    signal(SIGTSTP, ctrl_z_handler);
    char tempChar;
    int pipeCount = 0;
    int len=strlen(input);
    
    int redir_or_not = 0;
    int pipe_or_not = 0;
    if (strchr(input, '>') != NULL || strchr(input, '<') != NULL){
        redir_or_not=1;
    }
    if (strchr(input, '|') != NULL){
        pipe_or_not=1;
    }

    if (!(redir_or_not) && !(pipe_or_not)){
        char *token = strtok(input, " "); // Tokenize by space
        int i = 0;
        char **cmd = malloc(100 * sizeof(char*)); // Allocate space for 100 char* pointers

        while (token != NULL) {
            cmd[i] = malloc(strlen(token) + 1); // Allocate space for each token
            if (cmd[i] == NULL) {
                perror("malloc");
                exit(1);
            }

            strcpy(cmd[i], token); // Copy the token to cmd
            i++;

            token = strtok(NULL, " "); // Get the next token
        }

        executePipedCommandStrings(cmd, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
        return;
    }

    int didIT=0;
    for (int i=len-1; i>=0; i--){
        tempChar= input[i];
        if (tempChar == '>'){
            input[len]='|'; input[len+1]='s'; input[len+2]='l'; input[len+3]='e'; 
            input[len+4]='e'; input[len+5]='p'; input[len+6]=' '; input[len+7]='0'; 
            input[len+8]='\0';
            pipeCount = 0;
            didIT=1;
            break;
        }
        if (tempChar == '|'){
            pipeCount = 0;
            break;
        }
    }

// TEST CASE FOR BELOW: cat<output.txt|grep trying THIS DOESN'T GIVE OUTPUT TO TERMINAL SO ADD CAT

    if (!(didIT)){
        for (int i=len-1; i>=0; i--){
            tempChar= input[i];
            if (tempChar == '>' || tempChar == '>'){
                break;
            }
            if (tempChar == '|'){
                input[len]='|'; input[len+1]='c'; input[len+2]='a'; input[len+3]='t'; 
                input[len+4]='\0';
                break;
            }
        }    
    }

// +++++++++++++++---ENDS HERE---+++++++++++++++++

// ++++++ THIS ADDITIONS SUCH THAT IN EDGE CASES WHERE THERE IS ONLY SINGLE STATEMENT WITHOUT &, and WITHOUT PIPING (i=0 AND i=num_proc-1) THEN OUTPUT IS STILL REDIRECTED TO THE PIPE.
// SO THAT THIS DOESN'T HAPPEN, I HAVE GIVEN THE PIPED OUTPUT TO cat, ESSENTIALLY PRINTING THE PIPED OUTPUT ONTO THE TERMINAL, MEANING STDOUT.

    len=strlen(input);
    int O_REDIR=0;
    int I_REDIR=0;
    int IS_PIPE=0;
    for (int i=len-1; i>=0; i--){
        tempChar= input[i];
        if (tempChar == '>' || (tempChar == '>' && input[i+1] == '>')){
            // input[len]='|'; input[len+1]='s'; input[len+2]='l'; input[len+3]='e'; 
            // input[len+4]='e'; input[len+5]='p'; input[len+6]=' '; input[len+7]='0'; 
            // input[len+8]='\0';
            O_REDIR=1;
            break;
        }
        if (tempChar == '|'){
            IS_PIPE=1;
            break;
        }
    }
    if (!(O_REDIR)){
        if (!(IS_PIPE)){
            input[len]='|'; input[len+1]='c'; input[len+2]='a'; input[len+3]='t'; 
            input[len+4]='\0';
        }
    }  

// --------------+++++++ ENDS HERE +++++++____________

    char*** cmd = (char***)malloc(100 * sizeof(char**));
    for (int i = 0; i < 100; i++) {
        cmd[i] = (char**)malloc(100 * sizeof(char*));
        for (int j=0; j<100; j++){
            cmd[i][j]= (char*)malloc(4096*sizeof(char));
        }
    }

    char** rawCommands= (char**)malloc(100*sizeof(char*));
    for (int i =0; i<100; i++){
        rawCommands[i]=(char*)malloc(10000*sizeof(char));
    }
    
    for (int i=0; i<strlen(input); i++){
        if (input[i] == '|'){
            pipeCount++;
        }
    }

    char* tempStr= strtok(input, "|");
    rawCommands[0]=tempStr;

    for (int i=0; i<pipeCount; i++){
        tempStr= strtok(NULL, "|");
        rawCommands[i+1]=tempStr;
    }

    tempStr= (char*)malloc(10000*sizeof(char));
    
    for (int i=0; i<=pipeCount; i++){

        char* triangle_bracket= (char*)malloc(1000*sizeof(char));
        char* beggining = &(rawCommands[i][0]);
        char tempChar;

        char** tempStrings= (char**)malloc(100*sizeof(char));
        int* null_indices= (int*)malloc(1000*sizeof(int));
        int angular_count = 0;
        int null_count = 0;
        strcpy(tempStr,rawCommands[i]);
        for (int j=0; j<strlen(rawCommands[i]); j++){
            tempChar= rawCommands[i][j];
            if (tempChar == '<' || tempChar == '>'){
                if (tempChar == '<'){
                    null_indices[null_count]=j;
                    null_count++;
                    tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
                    tempStrings[angular_count] = beggining;
                    beggining = &(rawCommands[i][j+1]);
                    triangle_bracket[angular_count]=tempChar;
                    angular_count++;
                }
                if (tempChar == '>'){
                    if (rawCommands[i][j+1] == '>'){
                        // tempStr[j]='\0';
                        // tempStr[j+1]='\0';
                        null_indices[null_count]=j;
                        null_count++;
                        null_indices[null_count]=j+1;
                        null_count++;
                        tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
                        tempStrings[angular_count] = beggining;
                        beggining = &(rawCommands[i][j+2]);
                        triangle_bracket[angular_count]='$';
                        angular_count++;
                        j++;
                    }
                    else{
                        // tempStr[j]='\0';
                        null_indices[null_count]=j;
                        null_count++;
                        tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
                        tempStrings[angular_count] = beggining;
                        beggining = &(rawCommands[i][j+1]);
                        triangle_bracket[angular_count]=tempChar;
                        angular_count++;
                    }
                }
            }
        }

        for (int j=0; j<null_count; j++){
            rawCommands[i][null_indices[j]]='\0';
        }

        tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
        tempStrings[angular_count] = beggining; 
        angular_count++;

        char** tempStringArr= (char**)malloc(angular_count*sizeof(char));

        for (int j=0; j<angular_count; j++){
            tempStringArr[j]= (char*)malloc(10000*sizeof(char));
            tempStringArr[j]=tempStrings[j];
        }

        char* token;
        int cnter = 0;
        for (int j=0; j<angular_count; j++){
            token = strtok(tempStringArr[j], " ");
            tempStrings[cnter] = (char*)malloc(10000*sizeof(char));
            tempStrings[cnter] = token;
            cnter++;
            while (1){
                token= strtok(NULL, " ");
                if (token == NULL){
                    break;
                }
                tempStrings[cnter] = (char*)malloc(10000*sizeof(char));
                tempStrings[cnter] = token;
                cnter++;
            }
            tempStrings[cnter] = (char*)malloc(10000*sizeof(char));
            if (triangle_bracket[j] == '<'){
                strcpy(tempStrings[cnter], "<");
                cnter++;
            }
            else if (triangle_bracket[j] == '>'){
                strcpy(tempStrings[cnter], ">");
                cnter++;
            }
            else if (triangle_bracket[j] == '$'){
                strcpy(tempStrings[cnter], ">>");
                cnter++;
            }
        }

        for (int j=0; j<cnter; j++){
            strcpy(cmd[i][j],tempStrings[j]);
        }
        cmd[i][cnter]=NULL;
    }

    // for (int i = 0; i <= pipeCount; i++) {
    //     printf("Command %d:\n", i + 1);
    //     for (int j = 0; cmd[i][j] != NULL; j++) {
    //         printf("  Argument %d: %s\n", j + 1, cmd[i][j]);
    //     }
    // }

    int** file_descriptors= (int**)malloc(100*sizeof(int*));

    int num_proc = pipeCount+1;
    for (int i=0; i<num_proc; i++){
        file_descriptors[i]=(int*)malloc(2*sizeof(int));
        pipe(file_descriptors[i]);
    }

    for (int i = 0; i < num_proc; i++) {
        pid_t pid = fork();
        char* tempbuf = (char*)malloc(10000 * sizeof(char));

        if (pid == 0) {
            if (i == 0 && i != num_proc-1) {
                // Child process 1: ls -l
                close(file_descriptors[i][0]);
                for (int j=1; j<num_proc; j++){
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                dup2(file_descriptors[i][1], STDOUT_FILENO);
                close(file_descriptors[i][1]);
                // printf("%d: %s", i, cmd[i][0]);
                // printf("%d: %s", i, cmd[i][1]);
                // printf("%d: %s", i, cmd[i][1]);
                // HANDLE I/O REDIRECTION:
                handleIORedirection(cmd[i]);    
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror("execvp");
                exit(0);
            }
            else if (i == 0 && i == num_proc-1){
                for (int j=0; j<num_proc; j++){
                    if (j == i-1){
                        continue;
                    }
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                close(file_descriptors[i-1][0]);
                handleIORedirection(cmd[i]);
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror(cmd[i][0]);
                exit(0);    
            }
            else if (i < num_proc-1){
                // Child process 2: wc -l
                for (int j=0; j<num_proc; j++){
                    if (j == i || j == i-1){
                        continue;
                    }
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                close(file_descriptors[i-1][1]);
                close(file_descriptors[i][0]);
                dup2(file_descriptors[i-1][0], STDIN_FILENO);
                close(file_descriptors[i-1][0]);
                dup2(file_descriptors[i][1], STDOUT_FILENO);
                close(file_descriptors[i][1]);
                handleIORedirection(cmd[i]);
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror(cmd[i][0]);
                exit(0);
            } 
            else if (i == num_proc-1){
                // Child process 3: grep
                for (int j=0; j<num_proc; j++){
                    if (j == i || j == i-1){
                        continue;
                    }
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                close(file_descriptors[i][0]);
                close(file_descriptors[i][1]);
                dup2(file_descriptors[i-1][0], STDIN_FILENO);
                close(file_descriptors[i-1][0]);
                handleIORedirection(cmd[i]);
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror(cmd[i][0]);
                exit(0);
            }

        } 
        else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // ||||||||||-+-+-+---Close all pipes in the parent process---+-+-+-+-|||||||||||||||

    for (int i=0; i<num_proc; i++){
        close(file_descriptors[i][0]);
        close(file_descriptors[i][1]);
    }

    // Wait for all child processes to complete
    // for (int j = 0; j < num_proc; j++) {
    //     wait(NULL);
    // }
    wait(NULL);
    return;
}

void sortPipedCommands(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int bg, int* BG_PIDs, int* num_bg_proc){
    signal(SIGTSTP, ctrl_z_handler);
    char tempChar;
    int pipeCount = 0;
    int len=strlen(input);
    
    int didIT=0;
    for (int i=len-1; i>=0; i--){
        tempChar= input[i];
        if (tempChar == '>'){
            input[len]='|'; input[len+1]='s'; input[len+2]='l'; input[len+3]='e'; 
            input[len+4]='e'; input[len+5]='p'; input[len+6]=' '; input[len+7]='0'; 
            input[len+8]='\0';
            pipeCount = 0;
            didIT=1;
            break;
        }
        if (tempChar == '|'){
            pipeCount = 0;
            break;
        }
    }

// TEST CASE FOR BELOW: cat<output.txt|grep trying THIS DOESN'T GIVE OUTPUT TO TERMINAL SO ADD CAT

    if (!(didIT)){
        for (int i=len-1; i>=0; i--){
            tempChar= input[i];
            if (tempChar == '>' || tempChar == '>'){
                break;
            }
            if (tempChar == '|'){
                input[len]='|'; input[len+1]='c'; input[len+2]='a'; input[len+3]='t'; 
                input[len+4]='\0';
                break;
            }
        }    
    }

// +++++++++++++++---ENDS HERE---+++++++++++++++++

// ++++++ THIS ADDITIONIS SUCH THAT IN EDGE CASES WHERE THERE IS ONLY SINGLE STATEMENT WITHOUT &, and WITHOUT PIPING (i=0 AND i=num_proc-1) THEN OUTPUT IS STILL REDIRECTED TO THE PIPE.
// SO THAT THIS DOESN'T HAPPEN, I HAVE GIVEN THE PIPED OUTPUT TO cat, ESSENTIALLY PRINTING THE PIPED OUTPUT ONTO THE TERMINAL, MEANING STDOUT.

    len=strlen(input);
    int O_REDIR=0;
    int I_REDIR=0;
    int IS_PIPE=0;
    for (int i=len-1; i>=0; i--){
        tempChar= input[i];
        if (tempChar == '>' || (tempChar == '>' && input[i+1] == '>')){
            // input[len]='|'; input[len+1]='s'; input[len+2]='l'; input[len+3]='e'; 
            // input[len+4]='e'; input[len+5]='p'; input[len+6]=' '; input[len+7]='0'; 
            // input[len+8]='\0';
            O_REDIR=1;
            break;
        }
        if (tempChar == '|'){
            IS_PIPE=1;
            break;
        }
    }
    if (!(O_REDIR)){
        if (!(IS_PIPE)){
            input[len]='|'; input[len+1]='c'; input[len+2]='a'; input[len+3]='t'; 
            input[len+4]='\0';
        }
    }

// --------------+++++++ ENDS HERE +++++++++-------------------

    char*** cmd = (char***)malloc(100 * sizeof(char**));
    for (int i = 0; i < 100; i++) {
        cmd[i] = (char**)malloc(100 * sizeof(char*));
        for (int j=0; j<100; j++){
            cmd[i][j]= (char*)malloc(4096*sizeof(char));
        }
    }

    char** rawCommands= (char**)malloc(100*sizeof(char*));
    for (int i =0; i<100; i++){
        rawCommands[i]=(char*)malloc(10000*sizeof(char));
    }
    
    for (int i=0; i<strlen(input); i++){
        if (input[i] == '|'){
            pipeCount++;
        }
    }

    char* tempStr= strtok(input, "|");
    rawCommands[0]=tempStr;

    for (int i=0; i<pipeCount; i++){
        tempStr= strtok(NULL, "|");
        rawCommands[i+1]=tempStr;
    }

    tempStr= (char*)malloc(10000*sizeof(char));
    
    for (int i=0; i<=pipeCount; i++){

        char* triangle_bracket= (char*)malloc(1000*sizeof(char));
        char* beggining = &(rawCommands[i][0]);
        char tempChar;

        char** tempStrings= (char**)malloc(100*sizeof(char));
        int* null_indices= (int*)malloc(1000*sizeof(int));
        int angular_count = 0;
        int null_count = 0;
        strcpy(tempStr,rawCommands[i]);
        for (int j=0; j<strlen(rawCommands[i]); j++){
            tempChar= rawCommands[i][j];
            if (tempChar == '<' || tempChar == '>'){
                if (tempChar == '<'){
                    null_indices[null_count]=j;
                    null_count++;
                    tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
                    tempStrings[angular_count] = beggining;
                    beggining = &(rawCommands[i][j+1]);
                    triangle_bracket[angular_count]=tempChar;
                    angular_count++;
                }
                if (tempChar == '>'){
                    if (rawCommands[i][j+1] == '>'){
                        // tempStr[j]='\0';
                        // tempStr[j+1]='\0';
                        null_indices[null_count]=j;
                        null_count++;
                        null_indices[null_count]=j+1;
                        null_count++;
                        tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
                        tempStrings[angular_count] = beggining;
                        beggining = &(rawCommands[i][j+2]);
                        triangle_bracket[angular_count]='$';
                        angular_count++;
                        j++;
                    }
                    else{
                        // tempStr[j]='\0';
                        null_indices[null_count]=j;
                        null_count++;
                        tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
                        tempStrings[angular_count] = beggining;
                        beggining = &(rawCommands[i][j+1]);
                        triangle_bracket[angular_count]=tempChar;
                        angular_count++;
                    }
                }
            }
        }

        for (int j=0; j<null_count; j++){
            rawCommands[i][null_indices[j]]='\0';
        }

        tempStrings[angular_count] = (char*)malloc(10000*sizeof(char));
        tempStrings[angular_count] = beggining; 
        angular_count++;

        char** tempStringArr= (char**)malloc(angular_count*sizeof(char));

        for (int j=0; j<angular_count; j++){
            tempStringArr[j]= (char*)malloc(10000*sizeof(char));
            tempStringArr[j]=tempStrings[j];
        }

        char* token;
        int cnter = 0;
        for (int j=0; j<angular_count; j++){
            token = strtok(tempStringArr[j], " ");
            tempStrings[cnter] = (char*)malloc(10000*sizeof(char));
            tempStrings[cnter] = token;
            cnter++;
            while (1){
                token= strtok(NULL, " ");
                if (token == NULL){
                    break;
                }
                tempStrings[cnter] = (char*)malloc(10000*sizeof(char));
                tempStrings[cnter] = token;
                cnter++;
            }
            tempStrings[cnter] = (char*)malloc(10000*sizeof(char));
            if (triangle_bracket[j] == '<'){
                strcpy(tempStrings[cnter], "<");
                cnter++;
            }
            else if (triangle_bracket[j] == '>'){
                strcpy(tempStrings[cnter], ">");
                cnter++;
            }
            else if (triangle_bracket[j] == '$'){
                strcpy(tempStrings[cnter], ">>");
                cnter++;
            }
        }

        for (int j=0; j<cnter; j++){
            strcpy(cmd[i][j],tempStrings[j]);
        }
        cmd[i][cnter]=NULL;
    }


    // for (int i = 0; i <= pipeCount; i++) {
    //     printf("Command %d:\n", i + 1);
    //     for (int j = 0; cmd[i][j] != NULL; j++) {
    //         printf("  Argument %d: %s\n", j + 1, cmd[i][j]);
    //     }
    // }

    if (bg) {
        runInBG(cmd, pipeCount, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
    } 
    else {
        runInFG(cmd, pipeCount, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
    }
}

void runSortedPipedCommands(int pipeCount, char*** cmd, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc){
    signal(SIGTSTP, ctrl_z_handler);
    int** file_descriptors= (int**)malloc(100*sizeof(int*));

    int num_proc = pipeCount+1;
    for (int i=0; i<num_proc; i++){
        file_descriptors[i]=(int*)malloc(2*sizeof(int));
        pipe(file_descriptors[i]);
    }

    for (int i = 0; i < num_proc; i++) {
        pid_t pid = fork();
        char* tempbuf = (char*)malloc(10000 * sizeof(char));

        if (pid == 0) {
            if (i == 0 && i != num_proc-1) {
                // Child process 1: ls -l
                close(file_descriptors[i][0]);
                for (int j=1; j<num_proc; j++){
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                dup2(file_descriptors[i][1], STDOUT_FILENO);
                close(file_descriptors[i][1]);
                // printf("%d: %s", i, cmd[i][0]);
                // printf("%d: %s", i, cmd[i][1]);
                // printf("%d: %s", i, cmd[i][1]);
                // HANDLE I/O REDIRECTION:
                handleIORedirection(cmd[i]);
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror(cmd[i][0]);
                exit(0);
            }
            else if (i == 0 && i == num_proc-1){
                for (int j=0; j<num_proc; j++){
                    if (j == i || j == i-1){
                        continue;
                    }
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                close(file_descriptors[i][0]);
                close(file_descriptors[i][1]);
                dup2(file_descriptors[i-1][0], STDIN_FILENO);
                close(file_descriptors[i-1][0]);
                handleIORedirection(cmd[i]);
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror(cmd[i][0]);
                exit(0);
            }
            else if (i < num_proc-1){
                // Child process 2: wc -l
                for (int j=0; j<num_proc; j++){
                    if (j == i || j == i-1){
                        continue;
                    }
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                close(file_descriptors[i-1][1]);
                close(file_descriptors[i][0]);
                dup2(file_descriptors[i-1][0], STDIN_FILENO);
                close(file_descriptors[i-1][0]);
                dup2(file_descriptors[i][1], STDOUT_FILENO);
                close(file_descriptors[i][1]);
                handleIORedirection(cmd[i]);
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror(cmd[i][0]);
                exit(0);
            } 
            else if (i == num_proc-1){
                // Child process 3: grep
                for (int j=0; j<num_proc; j++){
                    if (j == i || j == i-1){
                        continue;
                    }
                    close(file_descriptors[j][0]);
                    close(file_descriptors[j][1]);
                }
                close(file_descriptors[i][0]);
                close(file_descriptors[i][1]);
                dup2(file_descriptors[i-1][0], STDIN_FILENO);
                close(file_descriptors[i-1][0]);
                handleIORedirection(cmd[i]);
                // execvp(cmd[i][0], cmd[i]);
                executePipedCommandStrings(cmd[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
                // perror(cmd[i][0]);
                exit(0);
            }

        } 
        else if (pid < 0) {
            perror("fork");
            exit(1);
        }
    }

    // ||||||||||-+-+-+---Close all pipes in the parent process---+-+-+-+-|||||||||||||||

    for (int i=0; i<num_proc; i++){
        close(file_descriptors[i][0]);
        close(file_descriptors[i][1]);
    }

    // Wait for all child processes to complete
    // for (int j = 0; j < num_proc; j++) {
    //     wait(NULL);
    // }
    wait(NULL);
    return;
}




// int main(){
//     char* input= (char*)malloc(10000*sizeof(char));
//     fgets(input, 10000, stdin);
//     input[strlen(input)-1] = '\0';
//     runPipedCommands(input);
// }
//              +++++++++ TEST CASES TEST CASES TEST CASES +++++++++

// 1:

    // strcpy(cmd[0][0], "ls");
    // strcpy(cmd[0][1], "-l");
    // cmd[0][2] = NULL;

    // strcpy(cmd[1][0], "grep");
    // strcpy(cmd[1][1], "trying");
    // cmd[1][2] = NULL;

    // strcpy(cmd[2][0], "cat");
    // strcpy(cmd[2][1], "-E");
    // cmd[2][2] = NULL;

    // strcpy(cmd[3][0], "wc");
    // strcpy(cmd[3][1], "-w");
    // cmd[3][2] = NULL;

// 2:

    // strcpy(cmd[0][0], "echo");
    // strcpy(cmd[0][1], "Hello, World!");
    // cmd[0][2] = NULL;

    // strcpy(cmd[1][0], "wc");
    // cmd[1][3]= NULL;

    // strcpy(cmd[1][0], "tr");
    // strcpy(cmd[1][1], "[:lower:]");
    // strcpy(cmd[1][2], "[:upper:]");
    // cmd[1][3] = NULL;

    // strcpy(cmd[2][0], "rev");
    // cmd[2][1] = NULL;

    // strcpy(cmd[3][0], "sed");
    // strcpy(cmd[3][1], "s/,/./");
    // cmd[3][2] = NULL;

    // strcpy(cmd[4][0], "awk");
    // strcpy(cmd[4][1], "{print");
    // strcpy(cmd[4][2], "$2}");
    // cmd[4][3] = NULL;