#include <iostream>
#include <algorithm>

using namespace std;

// Process struct defn'
struct Process
{
    int pid;            // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // Burst Time
    int remainingTime;  // 
    int finishTime;     // time when finished
};

void findWaitingTime(Process proc[], int numberOfProcesses, int waitTime[])
{
    // calculating waiting time, loop through all processes, add the lastproc waitime + lastproc burst time to get current proc wait time.
    for(int idx = 0; idx < numberOfProcesses; idx++)
    {
        // do the math
        waitTime[ idx ] = (proc[idx].finishTime - proc[idx].arrivalTime) - proc[idx].burstTime;
    }
}

void findTurnaroundTime(Process proc[], int numberOfProcesses, int waitTime[], int turnaroundTime[])
{
    // loop through all the other processes, calculate burst time by adding the waittime of the proc + the burst time of the proc.
    for(int idx = 0; idx < numberOfProcesses; idx++)
    {
        // do the math
        turnaroundTime[ idx ] = proc[idx].finishTime - proc[idx].arrivalTime;
    }
}

void displayProcessList(Process proc[], int numberOfProcesses, int waitTime [], int turnaroundTime [])
{
    // Write code to Display processes along with all details (Process ID, Burst Time, Waiting Time, Turnaround Time)
    printf("\nProcess   Finish Time   Turnaround Time   Waiting Time\n");
    // loop through all the processes, printing the information we've calculated about them.
    for(int idx = 0; idx < numberOfProcesses; idx++)
    {
        printf(" %3d \t      %3d \t     %3d \t     %3d\n" , proc[idx].pid , proc[idx].finishTime , turnaroundTime[idx] , waitTime[idx] );
    }
}

void findavgTime(Process proc[], int numberOfProcesses)
{
    int waitTime[numberOfProcesses], turnaroundTime[numberOfProcesses];

    // sort through process list to make them ascending by pid before executing
    std::sort( &proc[0] ,&proc[numberOfProcesses], [](Process a, Process b) {
        return a.pid < b.pid;   
    });

    // Your function to find turnaround time for all processes
    findTurnaroundTime(proc, numberOfProcesses, waitTime, turnaroundTime);
    // Your function to find waiting time of all processes
    findWaitingTime(proc, numberOfProcesses, waitTime);
    // Your function to display all details
    displayProcessList(proc, numberOfProcesses, waitTime, turnaroundTime);


    // Calculate Average Waiting Time
    // calc avg turnaround
    float avgWaitTime = 0;
    float avgTurnTime = 0;

    // loop through processes, adding all the waittimes and turnaround times
    for(int idx = 0; idx < numberOfProcesses; idx++)
    {
        avgWaitTime += (float)waitTime[idx];
        avgTurnTime += (float)turnaroundTime[idx];
    }
    // divide by the # of processes to get a mean.
    avgWaitTime /= numberOfProcesses;
    avgTurnTime /= numberOfProcesses;
    
    printf("Average waiting time \t= %.2f ms\n",avgWaitTime);
    printf("Average turnaround time = %.2f ms\n",avgTurnTime);
    
}

void sorting(Process proc[] , int startPos , int numberOfProcesses)
{
    // Implement a basic sort algorithm which sorts only on the Burst Time value.
    // insertion sort
    // define temporary and loop index values;
    int sortIdx, cmpIdx;
    Process tmpVal;
    for(int sortIdx = (startPos + 1); sortIdx < numberOfProcesses; sortIdx++)
    {
        // start comparing all elements between 1 and sortIdx.
        cmpIdx = sortIdx;
        
        // sort between 1 to sortIdx, decrementing each time an insertion is made.
        while( (cmpIdx > 0 ) && ( proc[ cmpIdx - 1 ].remainingTime > proc[ cmpIdx ].remainingTime ) ){
            tmpVal = proc[ cmpIdx ];
            proc[ cmpIdx ] = proc[ cmpIdx - 1 ];
            proc[ cmpIdx - 1 ] = tmpVal;
            cmpIdx--;
        }
    }
}

int addProcessByTime( Process procList[] , int elapsed ,int numberOfProcesses , int arriv[] , int burst[] )
{
    int addedProcesses = 0;

    for(int i = 0; i < numberOfProcesses; i++)
    {
        if ( arriv[i] == elapsed ) {
            procList[ i ] = Process{ i , arriv[i] , burst[i], burst[i] };
            addedProcesses++;
        }
    }
    
    return addedProcesses;
}

int executeProcess( Process* proc , int elapsed )
{
    int removedProcesses = 0;

    // decrement remaining time by 1ms
    proc->remainingTime--;

    // check if proc is finished
    if ( proc->remainingTime==0 ) {
        removedProcesses++;
        
        proc->finishTime = elapsed + 1;
    }

    return removedProcesses;
}

// provided main
int main()
{
    int arriv[] = {0,1,2,3,4,5};
    int burst[] = {7,5,3,1,2,1}; 
    // int burst[] = {8,4,2,1,3,2}; // these are from the screenshot
    int numberOfProcesses = sizeof arriv / sizeof arriv[0];

    // Generate Process structs
    Process proc[numberOfProcesses] ;

    // elapsed represents the amount of time that has elapsed ( 1 iteration of loop = 1 unit of time = elapsed +1)
    int elapsed = 0;
    // processCount and finishedProcesses represent the end and start bounds (respectively) of the proc array
    int processCount = 0;
    int finishedProcesses = 0;

    while( finishedProcesses < numberOfProcesses ){
        // check for processes to add, and add them if so.
        processCount += addProcessByTime( proc , elapsed , numberOfProcesses, arriv, burst);

        // sort currently added processes
        sorting( proc , finishedProcesses , processCount );

        // Get process to execute, will be first in array after sorting
        Process* currentProc = &proc[finishedProcesses];

        // execute the selected process
        finishedProcesses += executeProcess( currentProc , elapsed );

        elapsed ++;
    }

    // calc avg times.
    findavgTime(proc, numberOfProcesses);
    printf("\n");
    
    return 0;
}
