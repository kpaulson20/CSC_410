// Orphan process - parent terminates early

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed.\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process with PID %d, parent PID is %d.\n", getpid(), getppid());
        sleep(5);  // Simulate work in the child
        printf("Child process with PID %d, new parent PID is %d.\n", getpid(), getppid());
    } else {
        // Parent process
        printf("Parent process with PID %d is exiting.\n", getpid());
        return 0;  // Parent exits, making the child an orphan
    }

    return 0;
}
