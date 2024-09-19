// Waiting for child process

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> // to use wait()

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed.\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Hi from Child process with PID %d.\n", getpid());
        sleep(2);  // Simulating some work in the child
        printf("Child process finished.\n");
    } else {
        // Parent process
        printf("Hi from Parent process with PID %d, waiting for child.\n", getpid());
        wait(NULL);  // Parent waits for the child to finish
        printf("Child has finished, parent can now proceed.\n");
    }

    return 0;
}
