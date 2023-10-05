#include "headers.h"

void disableRawMode() {
    struct termios raw;
    if (tcgetattr(STDIN_FILENO, &raw) == -1) {
        perror("tcgetattr");
        exit(1);
    }

    raw.c_lflag |= ICANON | ECHO;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

void enableRawMode() {
    struct termios raw;
    if (tcgetattr(STDIN_FILENO, &raw) == -1) {
        perror("tcgetattr");
        exit(1);
    }
    atexit(disableRawMode);

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        exit(1);
    }
}

int readMostRecentPID() {
    char loadavg[1024];
    FILE *loadavg_file = fopen("/proc/loadavg", "r");

    if (loadavg_file == NULL) {
        perror("fopen");
        exit(1);
    }

    if (fgets(loadavg, sizeof(loadavg), loadavg_file) == NULL) {
        perror("fgets");
        exit(1);
    }

    fclose(loadavg_file);

    char *lastNumberStr = NULL;

    // Find the last space character
    char *space = strrchr(loadavg, ' ');
    int lastNumber;

    if (space != NULL) {
        lastNumberStr = space + 1;
        lastNumber = atoi(lastNumberStr);
    }

    // Parse the load average file to extract the PID
    int pid = lastNumber;
    return pid;
}

void neonate(char* time_arg) {
    sig_atomic_t stop = 0;
    enableRawMode();

    int prev_pid = -1;
    char input_buffer[1024];
    int input_length = 0;

    while (!stop) {
        int current_pid = readMostRecentPID();
        printf("%d\n", current_pid);
        prev_pid = current_pid;

        // Read user input into the buffer
        int read_result = read(STDIN_FILENO, input_buffer + input_length, sizeof(input_buffer) - input_length - 1);

        if (read_result > 0) {
            input_length += read_result;
            input_buffer[input_length] = '\0';

            // Search for "x" to exit
            if (strstr(input_buffer, "x") != NULL) {
                stop = 1;
            }

            // Handle other input
        }

        sleep(atoi(time_arg));  // Adjust the sleep interval as needed
    }

    disableRawMode();
    return;
}