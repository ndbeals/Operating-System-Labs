#include <iostream>
#include <algorithm>
#include <functional>
#include <queue>

using namespace std;


// Process struct defn'
struct Process
{
    int pid;            // Process ID
    int arrivalTime;    // Arrival time
    int burstTime;      // Burst Time
    int remainingTime;  // 
    int completionTime; // time when finished
    int waitingTime;
    int turnaroundTime; 
};


int timeElapsed = 0;
int numberOfProcesses = 0;
int finishedProcesses = 0;
int processCount = 0;
// Process procList[];

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
        turnaroundTime[ idx ] = proc[idx].completionTime - proc[idx].arrivalTime;
    }
}

void displayProcessList(Process proc[], int numberOfProcesses, int waitTime [], int turnaroundTime [])
{
    // Write code to Display processes along with all details (Process ID, Burst Time, Waiting Time, Turnaround Time)
    printf("\nProcess    AT    BT   Complete Time   Turnaround Time   Waiting Time\n");
    // loop through all the processes, printing the information we've calculated about them.
    for(int idx = 0; idx < numberOfProcesses; idx++)
    {
        printf(" %3d \t  %3d   %3d       %3d              %3d             %3d\n" , proc[idx].pid , proc[idx].arrivalTime , proc[idx].burstTime , proc[idx].completionTime , turnaroundTime[idx] , waitTime[idx] );
    }
}

void findavgTime(Process proc[], int numberOfProcesses)
{
    int waitTime[numberOfProcesses], turnaroundTime[numberOfProcesses];

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
    
    printf("\nTotal turnaround time   = %.2f ms\n",avgTurnTime);
    avgTurnTime /= numberOfProcesses;
    printf("Average turnaround time = %.2f ms\n",avgTurnTime);
    printf("Total waiting time \t= %.2f ms\n",avgWaitTime);
    // divide by the # of processes to get a mean.
    avgWaitTime /= numberOfProcesses;
    printf("Average waiting time \t= %.2f ms\n",avgWaitTime);
    
}

bool sortByPID( Process a , Process b )
{
    return a.pid < b.pid;
}

// Simple sort to sort by finish times, to display the process list in chronological order
bool sortByArrivalTime( Process a , Process b )
{
    return a.arrivalTime > b.arrivalTime;
}

// each milisecond, check to see if there are processes to add.
int addProcessByTime( Process procList[] , int alreadyAddedProcesses ,int numberOfProcesses , int arriv[] , int burst[] )
{
    int addedProcesses = 0;

    // loop through possible process list, and add eligible ones to the actual list
    for(int i = alreadyAddedProcesses; i < numberOfProcesses; i++)
    {
        if ( arriv[i] <= timeElapsed ) {
            procList[ i ] = Process{ i + 1 , arriv[i] , burst[i], burst[i] ,0,0,0};
            addedProcesses++;
        }
    }
    
    return addedProcesses;
}

int executeProcess( Process* proc )
{
    if ( proc->remainingTime==0 ) { // trying to execute an already finished (and handled) process, fail gracefully
        return 0;
    }
    timeElapsed++; // increment the time elapsed
    
    int removedProcesses = 0;

    // decrement remaining time by 1ms
    proc->remainingTime--;
    
    // check if proc is finished
    if ( proc->remainingTime==0 ) {
        removedProcesses++;
        proc->completionTime = timeElapsed;
    }

    return removedProcesses;
}

Process* findLargestRemainingTime( Process procList[] , int finished ,int numberOfProcesses )
{
    int RT = -1000;
    Process* lowest;

    for(int pid = 0; pid < numberOfProcesses; pid++)
    {
        if ( procList[pid].remainingTime > RT && procList[pid].remainingTime>0) {
            lowest = &procList[pid];

            RT = lowest->remainingTime;
        }
    }

    for(int pid = 0; pid < numberOfProcesses; pid++)
    {
        if ( lowest->remainingTime == procList[pid].remainingTime && procList[pid].remainingTime>0 ) {
            if ( procList[pid].arrivalTime < lowest->arrivalTime ) {
                lowest = &procList[pid];
            }
        }
    }

    return lowest;        
}

int main()
{
    int arriv[] = {1,2,3,4};
    int burst[] = {2,4,6,8}; 
    int numberOfProcesses = sizeof arriv / sizeof arriv[0];

    timeElapsed = 1;
    finishedProcesses = 0;
    processCount = 0;

    // Generate Process structs
    Process procList[numberOfProcesses] ;

    while( finishedProcesses < numberOfProcesses ){
        // check for processes to add, and add them if so.
        processCount += addProcessByTime( procList , processCount , numberOfProcesses, arriv, burst);

        std::sort( &procList[finishedProcesses] , &procList[processCount] , sortByArrivalTime);

        // execute process
        Process* proc = findLargestRemainingTime( procList , finishedProcesses , processCount );
        finishedProcesses += executeProcess( proc );
        
        printf("Process: %d at time interval: %d\n",proc->pid,timeElapsed-1);
    }
    
    // sort by finish times so the printed list looks nice
    std::sort( &procList[0], &procList[numberOfProcesses] , sortByPID );
    // std::sort( &proc[0], &proc[numberOfProcesses] , sortByFinishTime );
    // calc avg times.
    findavgTime(procList, numberOfProcesses);
    printf("\n");

    return 0;    
}
