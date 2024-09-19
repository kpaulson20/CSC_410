// Do some work now

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
        printf("Child process (PID: %d) is working...\n", getpid());
        
        int sum = 0;
        for (int i = 1; i <= 10; i++) {
            sum += i;
            sleep(1);  
        }
        
        printf("Child process computed sum from 1 to 10: %d\n", sum);
    } else {
        // Parent process 
        printf("Parent process (PID: %d) is working concurrently with child...\n", getpid());

        int sum = 0;
        for (int i = 11; i <= 20; i++) {
            sum += i;
            sleep(1);  
        }

        printf("Parent process computed sum from 11 to 20: %d\n", sum);
        
        // Wait for child
        wait(NULL);  
    }

    return 0;
}
