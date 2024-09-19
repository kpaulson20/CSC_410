// Multiple child processes

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int n = 4;  // Number of child processes to create
    pid_t pid;

    for (int i = 0; i < n; i++) {
        pid = fork();
        if (pid == 0) {
            // Child process
            printf("Hi from Child process %d with PID %d.\n", i+1, getpid());
            sleep(1);  // Simulate some work
            printf("Child process %d finished.\n", i+1);
            return 0;  // Child process exits
        }
    }

    // Parent process waits for all children
    while (wait(NULL) > 0);
    printf("All children have finished.\n");

    return 0;
}
