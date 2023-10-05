#include "headers.h"


void handleInput(char* input, int* isFirstTime, char* home_dir, char* prev_dir_1, char* prev_dir_2, char* shell_PID, char** pasteventsarr, int* start, int* cur, int* arrlen){

    char* token= (char*)malloc(4096*sizeof(char));
    char* input_str= (char*)malloc(4096*sizeof(char));
    strcpy(input_str, input);
    char** colon_tokenized= (char**)malloc(1000*sizeof(char));
    int colon_count=0;
    colon_tokenized[0]= (char*)malloc(4096*sizeof(char));
    colon_tokenized[0]= input_str;
    colon_count++;

    int str_length= strlen(input_str);
    for (int i=0; i<str_length; i++){
        if (input_str[i] == ';'){

            input_str[i]='\0';
            if (i+1 < str_length){
                colon_tokenized[colon_count]= &(input_str[i+1]);
                colon_count++;
            }
        }   
    }

    int amp_count=0;
    char** amp_tokenized= (char**)malloc(1000*sizeof(char));

    int max_background_processes = 100;
    int max_arguments = 100;
    int max_argument_length = 4096;

    // Allocate memory for background processes array
    char ***background_processes_array = malloc(max_background_processes * sizeof(char**));
    for (int i = 0; i < max_background_processes; i++) {
        background_processes_array[i] = malloc(max_arguments * sizeof(char*));
        for (int j = 0; j < max_arguments; j++) {
            background_processes_array[i][j] = malloc(max_argument_length * sizeof(char));
        }
    }

    // Allocate memory for foreground processes array
    char **foreground_processes_array = malloc(max_arguments * sizeof(char*));
    for (int i = 0; i < max_arguments; i++) {
        foreground_processes_array[i] = malloc(max_argument_length * sizeof(char));
    }

    int* num_bg_processes;
    int* num_bg_proc_args;
    
    for (int i=0; i<colon_count; i++){
        amp_count=0;
        char* token= colon_tokenized[i]; 
        amp_tokenized[amp_count]= (char*)malloc(4096*sizeof(char));
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

        num_bg_processes= (int*)malloc(sizeof(int));
        num_bg_proc_args= (int*)malloc((amp_count-1)*sizeof(int));

        *num_bg_processes=0;
        for (int k=0; k<amp_count-1; k++){
            num_bg_proc_args[k]=0;
        }

        for (int j=0; j<amp_count-1; j++){
            identify_and_execute(amp_tokenized[j], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 1, background_processes_array, foreground_processes_array, num_bg_processes, num_bg_proc_args);
        }
        identify_and_execute(amp_tokenized[amp_count-1], isFirstTime, home_dir, prev_dir_1, prev_dir_2, shell_PID, pasteventsarr, start, cur, arrlen, 0, background_processes_array, foreground_processes_array, num_bg_processes, num_bg_proc_args);
    
        // for (int l = 0; l < *num_bg_processes; l++) {
        //     printf("Background Process %d:\n", l);
        //     for (int j = 0; j < num_bg_proc_args[l]; j++) {
        //         printf("  Arg[%d]: %s\n", j, background_processes_array[l][j]);
        //     }
        //     printf("  Arg Count: %d\n\n", num_bg_proc_args[l]);
        // }

        // // Print foreground_processes_array
        // printf("Foreground Process:\n");
        //     for (int l = 0; foreground_processes_array[l] != NULL; l++) {
        //         printf("  Arg[%d]: %s\n", l, foreground_processes_array[l]);
        // }

        runBackgroundProcesses(foreground_processes_array, background_processes_array, num_bg_processes, num_bg_proc_args);
    }

    return;
}

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