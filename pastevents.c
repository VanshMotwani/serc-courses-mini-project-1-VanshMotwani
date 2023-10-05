#include "headers.h"

void add_in_array(char** pasteventsarr, char* newelem, int* arrlen, int* start, int* cur){
    if (*(arrlen) == 0){
        *(start)= 0;
        *(cur)= 0;
        strcpy(pasteventsarr[0],newelem);
        *(arrlen)=*(arrlen)+1;
        if (*(arrlen) > 15){
            (*arrlen)=15;
        }
    }
    else{
        *(cur)= (*(cur)+1)%15;
        if (*(cur) == *(start)){
            *(start)= (*(start)+1)%15;
        }
        strcpy(pasteventsarr[*(cur)],newelem);
        *(arrlen)=*(arrlen)+1;
        if (*(arrlen) > 15){
            (*arrlen)=15;
        }
    }

    FILE* myFile = fopen("pastevents.txt", "w");
    if (!myFile) {
        perror("Error opening pastevents.txt\n");
        return;
    }

    for (int i = *(start); i != *(cur); i = (i + 1) % 15) {
        fprintf(myFile, "%s\n", pasteventsarr[i]);
    }
    fprintf(myFile, "%s\n", pasteventsarr[*(cur)]);

    fclose(myFile);
}

void populate_array_from_file(char** pasteventsarr, int* arrlen, int* start, int* cur) {
    FILE *myFile = fopen("pastevents.txt", "r");
    if (!myFile) {
        printf("Error opening pastevents.txt");
        return;
    }

    char line[4096];
    *cur = 0;

    while (fgets(line, sizeof(line), myFile)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character if present

        // Allocate memory for the line and copy it to the array
        pasteventsarr[*cur] = malloc(strlen(line) + 1);
        if (pasteventsarr[*cur]) {
            strcpy(pasteventsarr[*cur], line);
            *cur = (*cur + 1) % 15;
            (*arrlen)++;
            if (*(arrlen) > 15){
            (*arrlen)=15;
            }
        } else {
            printf("Memory allocation error");
            break;
        }

        if (*cur == *start) {
            *start = (*start + 1) % 15;
        }
    }

    fclose(myFile);
}

void add_to_pastevents_arr(char* input, char** pasteventsarr, char* prevInput, int* arrlen, int* start, int* cur, char* newinput, int* isPastExecPresent){
    char* copy_token= (char*)malloc(4096*sizeof(char));
    char* copy_input= (char*)malloc(4096*sizeof(char));
    strcpy(copy_input, input);
    copy_token = strtok(copy_input, ";");
    
    populate_pastevents_arr(copy_token, pasteventsarr, prevInput, arrlen, start, cur, newinput, isPastExecPresent);

    while(1){
        copy_token = strtok(NULL, ";");
        if (copy_token == NULL){
            break;
        }
        populate_pastevents_arr(copy_token, pasteventsarr, prevInput, arrlen, start, cur, newinput, isPastExecPresent);
    }
}

void populate_pastevents_arr(char* input, char** pasteventsarr, char* prevInput, int* arrlen, int* start, int* cur, char* newinput, int* isPastExecPresent){
    int pasteventIndex=0;
    int pastevents_was_previous_token_only_spaces_since_then=0;
    int pastexecEndIndices[100];
    int pastexecStartIndices[100];

    int pasteventsPresenceFlag=0;
    int pasteventsExecFlag=0;
    int pastexecArguements[100];
    for (int i=0; i<100; i++){
        pastexecArguements[i] = -1;
    }

    for (int i=0; i<strlen(input); i++){
        if (((i<strlen(input)) && input[i] == 'p') && ((i+1<strlen(input)) && input[i+1] == 'a') && ((i+2<strlen(input)) && input[i+2] == 's') && ((i+3<strlen(input)) && input[i+3] == 't') && ((i+4<strlen(input)) && input[i+4] == 'e') && ((i+5<strlen(input)) && input[i+5] == 'v') && ((i+6<strlen(input)) && input[i+6] == 'e') && ((i+7<strlen(input)) && input[i+7] == 'n') && ((i+8<strlen(input)) && input[i+8] == 't') && ((i+9<strlen(input)) && input[i+9] == 's')){
            pasteventIndex=i;
            pasteventsPresenceFlag+=1;
            i=i+9;
            pastevents_was_previous_token_only_spaces_since_then=1;
        }
        else if (((i<strlen(input)) && input[i] == 'e') && ((i+1<strlen(input)) && input[i+1] == 'x') && ((i+2<strlen(input)) && input[i+2] == 'e') && ((i+3<strlen(input)) && input[i+3] == 'c') && ((i+4<strlen(input)) && input[i+4] == 'u') && ((i+5<strlen(input)) && input[i+5] == 't') && ((i+6<strlen(input)) && (input[i+6] == 'e'))){
            if (pastevents_was_previous_token_only_spaces_since_then){
                pasteventsExecFlag+=1;
                // pastexecEndIndices[pasteventsExecFlag]=i+6;
                pastexecStartIndices[pasteventsExecFlag]=pasteventIndex;

                for (int j=i+7; j<strlen(input); j++){
 
                    if (input[j] == ' '){
                        //
                    }
                    else{
                        int num_arg;
                        if (((j+1) < strlen(input)) && ((int)input[j+1])>=48 && ((int)input[j+1])<=57){
                            char* num_arguement= (char*)malloc(3*sizeof(char));
                            num_arguement[0]= input[j];
                            num_arguement[1]= input[j+1];
                            num_arguement[2]= '\0';
                            // printf("num_arguement:%s\n", num_arguement);
                            int num_arg= atoi(num_arguement);
                            pastexecArguements[pasteventsExecFlag]=num_arg;
                            pastexecEndIndices[pasteventsExecFlag]=j+1;
                            break;
                        }
                        else{
                            pastexecArguements[pasteventsExecFlag]=((int)input[j])-(int)'0';
                            pastexecEndIndices[pasteventsExecFlag]=j;

                            break;
                        }
                    }
                }
            }
        }
        else if (input[i] == ' '){
            // do nothing
        }
        else{
            pastevents_was_previous_token_only_spaces_since_then=0;
        }
    }

    char newinput1[4096];
    char newinput2[4096];
    char newinput3[4096];

    if (pasteventsExecFlag){
        for (int i=1; i<=pasteventsExecFlag; i++){
            input[pastexecStartIndices[i]]='\0';
            strcpy(newinput3, &(input[pastexecEndIndices[i] + 1]));

            if (pastexecArguements[i] > 15 || pastexecArguements[i] < 0){
                strcpy(newinput2, "Invalid Arguement");
            }
            int count=0;
            int index_of_arg=-1;
            int found_flag=0;

            for (index_of_arg=*(start); index_of_arg!=*(cur); index_of_arg=(index_of_arg+1)%15){
                // if (count == pastexecArguements[*(cur)]){
                //     break;
                // }

                if (((*arrlen)-count) == pastexecArguements[i]){
                    found_flag=1;
                    break;
                }
                count++;
            }

            // printf("start:%d cur:%d\n", *(start), *(cur));
            // if (count == pastexecArguements[i])
            // for (index_of_arg=*(start); index_of_arg!=*(cur); index_of_arg=(index_of_arg+1)%15){
            //     if 
            //     printf("%d:%s\n", i, pasteventsarr[i]);
            // }
            // printf("%d:%s\n",*(cur), pasteventsarr[*(cur)]);


            if (found_flag == 1){
                strcpy(newinput2, pasteventsarr[index_of_arg]);
            }
            else{
                snprintf(newinput2, sizeof(newinput2), "pastevents execute %d", pastexecArguements[i]);
            }

            strcat(input, newinput2);
            strcat(input, newinput3);
            for (int j=i+1; j<=pasteventsExecFlag; j++){
                pastexecStartIndices[j]+=(strlen(newinput2)-(pastexecEndIndices[i]-pastexecStartIndices[i]+1));
                pastexecEndIndices[j]+=(strlen(newinput2)-(pastexecEndIndices[i]-pastexecStartIndices[i]+1));
            }
        }

        strcpy(newinput, input);
        *(isPastExecPresent)=1;
    }


    if (strcmp(input, "\0") != 0 && strcmp(input, "") != 0){
        if (strcmp(input, prevInput) != 0){
            if ((!pasteventsPresenceFlag) || (pasteventsExecFlag>=pasteventsPresenceFlag)){    // not pasteventsflag
                add_in_array(pasteventsarr, input, arrlen, start, cur);
                strcpy(prevInput, input);
            }
        }
    }
    
}