// Do some work with a global variable

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int mynum = 0;

int main(void)
{
   pid_t pid;
   pid = fork();

   if (pid == 0)  // child process
   {
      for (int i = 0; i < 5; i++)
      {
         mynum= i;
         sleep(1); 
         printf("Child %d\n", mynum);
      }
   }
   else if (pid > 0)  // parent process
   {
      for (int i = 0; i < 5; i++) 
      {
         sleep(1); 
         printf("Parent %d\n", mynum);
      }
   }
   
   return 0;
}