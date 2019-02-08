#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <algorithm>
#include <inttypes.h>


using namespace std;


int main(int argc, char const *argv[])
{
    // create pipe descriptors
    int pipefd[2];
    pipe(pipefd);

    // create input variables
    string input;
    int inputLength = 32;

    
    // fork the process, execution starts from here
    pid_t pid = fork(); // fork returns 0 for the child process and the child-pid for the parent process

    if (pid == 0)
    {
        // child process
        // close write descriptor
        close( pipefd[1] );

        // read length
        read( pipefd[0], &inputLength, sizeof(inputLength) );

        //read input string
        ssize_t out = read( pipefd[0], &input, inputLength );

        // print the input
        printf("Hello %s!\n",input.c_str());

        // close read descriptor
        close( pipefd[0] );
    }
    else if (pid > 0)
    {
        // parent process
        // close read descriptor
        close( pipefd[0] );

        // get input
        printf("Input a name: ");
        getline( cin,input );
        // calculate the length of the input string, in bits
        inputLength = input.length() * sizeof(input);

        // write output variables, first the length of the string, then the string.
        write( pipefd[1], &inputLength , sizeof(inputLength) );
        
        // write the actual string now
        write( pipefd[1],&input, inputLength+1 );

        // close write descriptor
        close( pipefd[1] );
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }

    return 0;
}
