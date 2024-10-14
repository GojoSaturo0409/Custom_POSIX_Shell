#include"neonate.h"

// Save original terminal attributes
struct termios initial_tty_settings;

// Restore original terminal settings
void reset_terminal_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &initial_tty_settings) == -1) {
        perror("Error restoring terminal mode");
        exit(EXIT_FAILURE);
    }
}

// Configure terminal to raw mode (no buffering, no echo)
void activate_raw_mode() {
    if (tcgetattr(STDIN_FILENO, &initial_tty_settings) == -1) {
        perror("Error getting terminal attributes");
        exit(EXIT_FAILURE);
    }

    // Ensure terminal settings are restored upon exit
    atexit(reset_terminal_mode);

    struct termios raw = initial_tty_settings;
    raw.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("Error setting raw terminal mode");
        exit(EXIT_FAILURE);
    }
}

// Fetch PID of the most recently active process
pid_t get_latest_process_pid() {
    FILE *loadavg_file = fopen("/proc/loadavg", "r");
    if (!loadavg_file) {
        perror("Error opening /proc/loadavg");
        exit(EXIT_FAILURE);
    }

    char data[128];
    if (fgets(data, sizeof(data), loadavg_file) == NULL) {
        perror("Error reading /proc/loadavg");
        fclose(loadavg_file);
        exit(EXIT_FAILURE);
    }

    fclose(loadavg_file);

    // Split the line and extract the 5th token (most recent PID)
    char *token = strtok(data, " ");
    for (int i = 0; i < 4; i++) {
        token = strtok(NULL, " ");
    }

    return (pid_t)atoi(token);
}

// Monitor and print recent process PIDs at intervals
void monitor_processes(int delay) {
    while (1) {
        sleep(delay);
        pid_t recent_pid = get_latest_process_pid();
        printf("Latest process PID: %d\n", recent_pid);
    }
}

// Spawn a child process to monitor recent PIDs at regular intervals
pid_t spawn_monitor(int delay) {
    pid_t child_proc = fork();

    if (child_proc < 0) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    if (child_proc == 0) {
        monitor_processes(delay); // Child monitors PIDs
        exit(0); // In case the loop ever breaks
    }

    return child_proc; // Return child process ID to parent
}

// Wait for 'x' key press to terminate monitoring
void handle_user_input(pid_t child_proc) {
    activate_raw_mode();

    while (1) {
        char input = getchar();
        if (input == 'x') {
            printf("Terminating...\n");
            kill(child_proc, SIGKILL); // Terminate child process
            break;
        }
    }

    // Restore terminal settings
    reset_terminal_mode();
}

// Main function to start process monitoring and handle user input
void neonate(int time_interval) {
    pid_t child_proc = spawn_monitor(time_interval); // Fork and start monitoring
    handle_user_input(child_proc);           // Handle user input in the parent process
}