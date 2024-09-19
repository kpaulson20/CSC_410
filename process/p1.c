// Creating a process with fork

#include <stdio.h>
#include <unistd.h> // to use fork()
#include <sys/types.h> // to use pid_t

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed.\n");
        return 1;
    } else if (pid == 0) {  
        // Child process
        printf("Hi from child process with PID %d.\n", getpid());
    } else {  
        // Parent process
        printf("Hi from parent process with PID %d, and child PID is %d.\n", getpid(), pid);
    }

    return 0;
}
