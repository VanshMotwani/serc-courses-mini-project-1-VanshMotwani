#include "headers.h"

void iMan(char* command_name) {    
    // Perform DNS resolution
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints); 
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    struct addrinfo *res;
    getaddrinfo("man.he.net", "80", &hints, &res);

    // Create and connect the socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sockfd, res->ai_addr, res->ai_addrlen);

    // Send HTTP GET request
    char request[100];
    snprintf(request, sizeof(request), "GET /man1/%s HTTP/1.1\r\nHost: man.he.net\r\n\r\n", command_name);
    write(sockfd, request, strlen(request));

    char buffer[10000];
    char bufChar;
    int NAME_OCCURED=0;

    while (read(sockfd, buffer, sizeof(buffer)) > 0) {
        // int i=0;
        // int not_within_tags=1;
        // while(1){
        //     if (i==10000){
        //         break;
        //     }
        //     bufChar=buffer[i];
        //     if (bufChar == '\0'){
        //         break;
        //     }
        //     if (bufChar == 'N'){
        //         if (buffer[i+1] == 'A' && buffer[i+2] == 'M' && buffer[i+3] == 'E' && buffer[i+4] == '\n'){
        //             NAME_OCCURED=1;
        //         }
        //     }
        //     if (NAME_OCCURED){
        //         if (not_within_tags){
        //             printf("%c", bufChar);
        //         }
        //     }
        //     if (bufChar == '<'){
        //         not_within_tags=0;
        //     }
        //     if (bufChar == '>'){
        //         not_within_tags=1;
        //     }
        //     i++;
        // }
        printf("%s", buffer);
    } 

    close(sockfd);

    if (NAME_OCCURED == 0){
        printf("ERROR\n          No such command\n");
    }

    return;
}