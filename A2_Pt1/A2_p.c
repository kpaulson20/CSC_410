#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Starting shield power level
int shield_power = 50;  

int main() {
    pid_t pid;

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);

    // Create 4 child processes - 4 different characters adjusting shield power
            // Luke increases the shield power by 25
            printf("Luke: Adjusting shields...\n");
            shield_power += 25;
            printf("Luke: Shield Power level now at %d%%\n", shield_power);

            // Han increases the shield power by 20
            printf("Han: Adjusting shields...\n");
            shield_power += 20;
            printf("Han: Shield Power level now at %d%%\n", shield_power);

            // Chewbacca increases the shield power by 30
            printf("Chewbacca: Adjusting shields...\n");
            shield_power += 30;
            printf("Chewbacca: Shield Power level now at %d%%\n", shield_power);

            // Leia increases the shield power by 15
            printf("Leia: Adjusting shields...\n");
            shield_power += 15;
            printf("Leia: Shield Power level now at %d%%\n", shield_power);

    // Check if process creation failed
        // -----> Write you code here
        if (pid < 0) {
            printf("Process Creation FAILED\n");
            return 1;
        } 

    // Make parent process wait for all child processes to complete
        // -----> Write you code here 
        while(wait(NULL) > 0);

    // Parent process reports final state
    printf("\nFinal shield power level on the Millennium Falcon: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return 0;
}
