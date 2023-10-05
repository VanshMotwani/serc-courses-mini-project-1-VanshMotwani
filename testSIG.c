#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Get the PID of the current process
    pid_t pid = getpid();
    printf("PID: %d\n", pid);

    int cnt = 0;

    // Print "1" every 5 seconds
    while (1) {
    	if (cnt%1000000000 == 0){
            printf("%d\n", cnt);
        }
        cnt++;
    }

    return 0;
}
