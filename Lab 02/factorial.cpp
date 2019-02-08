#include <iostream>
// #include <stdio.h>
#include <unistd.h>
// #include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <algorithm>
#include <inttypes.h>


using namespace std;

// Function to compute factorials
uint64_t factorial( int input )
{
    // declare our result
    uint64_t result = 1;
    
    for(uint64_t i = input; i > 1; i--)
    {
        // multiple the initial value by each term between 1 and the input
        result *= i;
    }
    
    return result;
}

int main(int argc, char const *argv[])
{
    // fork the process, execution starts from here
    pid_t pid = fork();

    if (pid == 0)
    {
        // child process
        string input;
        bool validInput = false;

        do
        {
            printf("Input a number to compute the factorial of: ");
            
            getline(cin,input);
            
            if ( !input.empty() && std::all_of(input.begin(),input.end(),::isdigit) ) {
                validInput = true;
            }
            else {
                printf("Invalid input, try again\n");
            }
        } while (!validInput);

        printf("\nInput: %s\n",input.c_str());

        exit(strtol(input.c_str(),NULL,0));
    }
    else if (pid > 0)
    {
        // parent process
        int input;
        wait(&input);
        input /= 255; //have to divde by 255 to get real result

        uint64_t result = factorial(input);

        printf("Factorial results: %s\n",std::to_string(result).c_str());
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        return 1;
    }

    return 0;
}

