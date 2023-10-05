#include "headers.h"


void handleInput(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen, int* BG_PIDs, int* num_bg_proc){
    signal(SIGTSTP, ctrl_z_handler);
    char* token= (char*)malloc(10000*sizeof(char));
    char* input_str= (char*)malloc(10000*sizeof(char));
    strcpy(input_str, input);
    char** colon_tokenized= (char**)malloc(1000*sizeof(char));
    int colon_count=0;
    colon_tokenized[0]= (char*)malloc(10000*sizeof(char));
    colon_tokenized[0]= input_str;
    colon_count++;

    int* to_null= (int*)malloc(100*sizeof(int));
    int toNULLCount = 0;

    int str_length= strlen(input_str);
    for (int i=0; i<str_length; i++){
        if (input_str[i] == ';'){

            // input_str[i]='\0';
            to_null[toNULLCount]=i;
            toNULLCount++;
            if (i+1 < str_length){
                colon_tokenized[colon_count]= &(input_str[i+1]);
                colon_count++;
            }
        }   
    }

    for (int i=0; i<toNULLCount; i++){
        input_str[to_null[i]]='\0';
    }
   
    // for (int i=0; i<colon_count; i++){
    //     printf("colon_tokenized[%d]: %s\n", i, colon_tokenized[i]);
    // }    

    for (int i=0; i<colon_count; i++){
        int amp_count=0;
        char** amp_tokenized= (char**)malloc(1000*sizeof(char));

        // int max_background_processes = 50;
        // int max_piped_procs = 50;
        // int max_arguments = 50;

        // // Allocate memory for background processes array
        // char ****background_processes_array = (char****)malloc(max_background_processes * sizeof(char***));
        // for (int k = 0; k < max_background_processes; k++) {
        //     background_processes_array[k] = (char***)malloc(max_piped_procs * sizeof(char**));
        //     for (int j = 0; j < max_piped_procs; j++) {
        //         background_processes_array[k][j] = (char**)malloc(max_arguments * sizeof(char*));
        //         for (int o=0; o<max_arguments; o++){
        //             background_processes_array[k][j][o]= (char*)malloc(10000*sizeof(char));
        //         }
        //     }
        // }

        // // Allocate memory for foreground processes array
        // char ***foreground_processes_array = (char***)malloc(max_piped_procs * sizeof(char**));
        // for (int j = 0; j < max_piped_procs; j++) {
        //     foreground_processes_array[j] = (char**)malloc(max_arguments * sizeof(char*));
        //     for (int k=0; k<max_arguments; k++){
        //         foreground_processes_array[j][k]= (char*)malloc(10000*sizeof(char));
        //     }
        // }

        // int* num_bg_processes;
        
        // amp_count=0;
        char* token= colon_tokenized[i]; 
        amp_tokenized[amp_count]= (char*)malloc(10000*sizeof(char));
        amp_tokenized[amp_count]= token;
        amp_count++;

        int token_length= strlen(token);
        for (int j=0; j<token_length; j++){
            if (token[j] == '&'){
                token[j]='\0';
                if (j+1 < token_length){
                    amp_tokenized[amp_count]= &(token[j+1]);
                    amp_count++;
                }
            }   
        }

        if (amp_count == 1){
            // HANDLE PIPING AND I/O redirection of SELF MADE FUNCTIONS INCLUDED
            char* newtok= (char*)malloc(10000*sizeof(char));
            strcpy(newtok, token);    
            runPipedCommands(newtok, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, BG_PIDs, num_bg_proc);
            continue;
        }

        // int num_bg_processes= 0;
        // num_bg_processes= (int*)malloc(sizeof(int));
        // *num_bg_processes = 0;

        // int* bg_pipeCounts= (int*)malloc(100*sizeof(int));
        // int* fg_pipeCount = (int*)malloc(sizeof(int));
        // *fg_pipeCount = 0;

        for (int j=0; j<amp_count-1; j++){
            char* newamp_tok= (char*)malloc(10000*sizeof(char));
            strcpy(newamp_tok, amp_tokenized[j]);
            // identify_and_execute(amp_tokenized[j], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1, background_processes_array, foreground_processes_array, num_bg_processes, num_bg_proc_args);
            sortPipedCommands(newamp_tok, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1, BG_PIDs, num_bg_proc);
            // identify_and_execute(amp_tokenized[j], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1, background_processes_array, foreground_processes_array, num_bg_processes, num_bg_proc_args);
        }
        char* newamp_tok2= (char*)malloc(10000*sizeof(char));
        strcpy(newamp_tok2, amp_tokenized[amp_count-1]);
        sortPipedCommands(newamp_tok2, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 0, BG_PIDs, num_bg_proc);
        // identify_and_execute(amp_tokenized[amp_count-1], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 0, background_processes_array, foreground_processes_array, num_bg_processes, num_bg_proc_args);

    // TRY TO PRINT FG AND BG ARRAYS

// THE NUMBER OF BACKGROUND PROCESSES MUST BE ATLEAST ONE AT THIS POINT
        // for (int i=0; i<*num_bg_processes; i++){
        //     runSortedPipedCommands(bg_pipeCounts[i], background_processes_array[i], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen);
        // }
        // runSortedPipedCommands(*fg_pipeCount, foreground_processes_array, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen);
        // // runBackgroundProcesses(foreground_processes_array, background_processes_array, num_bg_processes, num_bg_proc_args);
    }

    return;
}

        // jk = -1;
        // char* concatenated_input= (char*)malloc(10000*sizeof(char));
        // while (1){
        //     jk++;
        //     str= foreground_processes_array[jk];
        //     if (str == NULL){
        //         break;
        //     }
        //     strcat(str, " ");
        //     strcat(concatenated_input, str);
        // }
        // printf("This is it: %s\n", concatenated_input);
        // runPipedCommands(concatenated_input, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen);

// void handleInput(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen){

//     char* token= (char*)malloc(4096*sizeof(char));
//     char* input_str= (char*)malloc(4096*sizeof(char));
//     strcpy(input_str, input);
//     int count=0;
//     token= strtok(input_str, ";");
//     printf("%s\n", token);
//     count++;

//     char* and_delimiter_token= (char*)malloc(4096*sizeof(char));
//     char* second_last_token= (char*)malloc(4096*sizeof(char));
//     int ampersand_count=0;
//     int process_exec_count=0;

//     if (strstr(token, "&")== NULL){
//         // execute this command as a foreground process
//         identify_and_execute(token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 0);     
//     }
//     else{
//         ampersand_count=0; // initializing for each loop
//         process_exec_count=0;  // initializing for each loop
//         for (int i=0; i<strlen(token); i++){
//             if (token[i] == '&'){
//                 ampersand_count++;
//             }
//         }
//         and_delimiter_token= strtok(token, "&");
//         // run the process with & ahead of it in background
//         identify_and_execute(and_delimiter_token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1);
//         process_exec_count++;
        
//         while (and_delimiter_token != NULL){
//             and_delimiter_token= strtok(NULL, "&");
//             if (and_delimiter_token == NULL){
//                 break;
//             }
//             if (process_exec_count == ampersand_count){
//                 // run the last token of & delimiter as foreground process
//                 identify_and_execute(and_delimiter_token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 0);
//                 process_exec_count++;
//             }
//             else{
//                 // execute the rest in background
//                 identify_and_execute(and_delimiter_token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1);
//                 process_exec_count++;
//             }
//         }
//     }  

//     while (token != NULL){
//         token= strtok(NULL, ";");
//         if (token == NULL){
//             break;
//         }
//         if (strstr(token, "&") == NULL){
//             // no &s present, so execute this command in foreground
//             identify_and_execute(token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 0);     
//         }
//         else{
//             ampersand_count=0; // initializing for each loop
//             process_exec_count=0;  // initializing for each loop
//             for (int i=0; i<strlen(token); i++){
//                 if (token[i] == '&'){
//                     ampersand_count++;
//                 }
//             }
//             and_delimiter_token= strtok(token, "&");
//             // run the process with & ahead of it in background
//             identify_and_execute(and_delimiter_token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1);
//             process_exec_count++;
            
//             while (and_delimiter_token != NULL){
//                 and_delimiter_token= strtok(NULL, "&");
//                 if (and_delimiter_token == NULL){
//                     break;
//                 }
//                 if (process_exec_count == ampersand_count){
//                     // run the last token of & delimiter as foreground process
//                     identify_and_execute(and_delimiter_token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 0);
//                     process_exec_count++;
//                 }
//                 else{
//                     // execute the rest in background
//                     identify_and_execute(and_delimiter_token, isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1);
//                     process_exec_count++;
//                 }
//             }
//         }
//     }

//     return;
// }