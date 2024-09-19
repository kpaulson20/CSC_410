// Zombie process - parent does not wait

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed.\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process with PID %d is finishing.\n", getpid());
        return 0;  // Child finishes
    } else {
        // Parent process - no wait
        printf("Parent process with PID %d.\n", getpid());
        sleep(10);  
        printf("Parent process finished.\n");
    }

    return 0;
}
