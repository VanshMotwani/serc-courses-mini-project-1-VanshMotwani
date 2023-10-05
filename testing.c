#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int a=1; 
    int b=1;

    pid_t childPid = fork();

    if (childPid == 0) {
        // Child process
        
        // Execute the "sleep 5" command in the foreground
        // char *cmd = "sleep";
        // char *args[] = {"sleep", "5", NULL};
        // execvp(cmd, args);

        for (int i=0; i<1000; i++){
            if (i%31){
                printf("%d ", i);
            }
        }
        
        

        // perror("execvp"); // Print error if execvp fails
        // exit(1);
    } else if (childPid > 0) {
        // Parent process
        
        printf("Foreground process (sleep 5) is running...\n");
        
        // Fork again to create a new child process (grandchild)
        pid_t grandchildPid = fork();
        
        if (grandchildPid == 0) {
            // Grandchild process
            
            // Create a new session and become the leader of a new process group
            setsid();
            
            // Close standard file descriptors to disassociate from the terminal
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            
            char input[100]; // Buffer to store user input
            // Execute the "sleep 10" command in the background
            char *cmd = "sleep";
            char *args[] = {"sleep", "10", NULL};
            // execvp(cmd, args);
            // if (fgets(input, sizeof(input), stdin) != NULL) {
            //     printf("You entered: %s", input);
            // } else {
            //     perror("fgets");
            // }
            
            for (int i=0; i<1000; i++){
                printf("LL\n");
            }

            perror("execvp"); // Print error if execvp fails
            exit(1);
        } else if (grandchildPid < 0) {
            perror("fork");
            exit(1);
        }
        
        // Wait for the grandchild process to complete (optional)
        waitpid(grandchildPid, NULL, 0);
        
        // Wait for the foreground process to complete
        waitpid(childPid, NULL, 0);

        printf("Value on a:%d\n", a);

    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}