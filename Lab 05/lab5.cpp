#include <iostream>
#include <algorithm>
#include <functional>

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


void findWaitingTime(Process proc[], int numberOfProcesses, int waitTime[] , int turnaroundTime[])
{
    // calculating waiting time, loop through all processes, add the lastproc waitime + lastproc burst time to get current proc wait time.
    for(int idx = 0; idx < numberOfProcesses; idx++)
    {
        // do the math
        waitTime[ idx ] = turnaroundTime[idx] - proc[idx].burstTime;
    }
}

void findTurnaroundTime(Process proc[], int numberOfProcesses, int turnaroundTime[])
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
    // std::sort( &proc[0] ,&proc[numberOfProcesses], [](Process a, Process b) {
        // return a.pid < b.pid;   
    // });

    // Your function to find turnaround time for all processes
    findTurnaroundTime(proc, numberOfProcesses, turnaroundTime);
    // Your function to find waiting time of all processes
    findWaitingTime(proc, numberOfProcesses, waitTime, turnaroundTime);
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

// helper function to calculate waiting time
inline float waitingTime( Process proc , int elapsed)
{
    return elapsed - proc.arrivalTime;
}

// Sort by HRRN
bool sortByHRRN( Process a , Process b , int elapsed)
{
    // calculate response ratios
    float ratioA = 1.0f + float( waitingTime(a , elapsed) / a.burstTime);
    float ratioB = 1.0f + float( waitingTime(b , elapsed) / b.burstTime);

    // if response ratio of process A is higher than that of process B, then A should come before B
    return ratioA > ratioB; 
}

// Simple sort to sort by finish times, to display the process list in chronological order
bool sortByFinishTime( Process a , Process b )
{
    return a.finishTime < b.finishTime;
}

// each milisecond, check to see if there are processes to add.
int addProcessByTime( Process procList[] , int elapsed ,int numberOfProcesses , int arriv[] , int burst[] )
{
    int addedProcesses = 0;

    // loop through possible process list, and add eligible ones to the actual list
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
    // int arriv[] = {0,1,2,3,4,5};
    // int burst[] = {7,5,3,1,2,1}; 
    // int burst[] = {8,4,2,1,3,2}; // these are from the screenshot
    
    int arriv[] = {0,2,4,6,8};
    int burst[] = {3,6,4,5,2}; 
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
        // sorting( proc , finishedProcesses , processCount );
        std::sort( &proc[finishedProcesses], &proc[processCount] , std::bind(sortByHRRN, std::placeholders::_1 ,std::placeholders::_2, elapsed) );

        // Get process to execute, will be first in array after sorting
        Process* currentProc = &proc[finishedProcesses];

        // execute the selected process
        finishedProcesses += executeProcess( currentProc , elapsed );

        elapsed ++;
    }

    // sort by finish times so the printed list looks nice
    std::sort( &proc[0], &proc[numberOfProcesses] , sortByFinishTime );
    // calc avg times.
    findavgTime(proc, numberOfProcesses);
    printf("\n");
    
    return 0;
}
