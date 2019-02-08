#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


using namespace std;

int main(int argc, char const *argv[])
{
    printf("Forking Process!\n\n" );
    
    // fork the process, execution starts from here
    pid_t pid = fork(); // fork returns 0 for the child process and the child-pid for the parent process

    if (pid == 0)
    {
        // child process
        printf("Child Process ID: %d\n", (int) getpid());
    }
    else if (pid > 0)
    {
        // parent process
        printf("Parent PID: %d\n", (int) getpid());
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }

    // printf("End\n");

    return 0;
}
