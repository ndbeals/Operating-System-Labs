#include <iostream>
#include <algorithm>
#include <functional>
#include <queue>

using namespace std;

int TQ = 50; // Time Quantum global var.
int timeElapsed = 0;

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
    return timeElapsed - proc.arrivalTime;
}

bool sortByPID( Process a , Process b )
{
    return a.pid < b.pid;
}

// Simple sort to sort by finish times, to display the process list in chronological order
bool sortByFinishTime( Process a , Process b )
{
    return a.finishTime < b.finishTime;
}

// each milisecond, check to see if there are processes to add.
int addProcessByTime( Process procList[] , int alreadyAddedProcesses ,int numberOfProcesses , int arriv[] , int burst[] )
{
    int addedProcesses = 0;

    // loop through possible process list, and add eligible ones to the actual list
    for(int i = alreadyAddedProcesses; i < numberOfProcesses; i++)
    {
        if ( arriv[i] <= timeElapsed ) {
            procList[ i ] = Process{ i , arriv[i] , burst[i], burst[i] ,0};
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
        proc->finishTime = timeElapsed ;
    }

    return removedProcesses;
}

void buildProcessQueue( std::deque<Process*>& processQueue , Process proclist[], int start, int end )
{
    for(int proc = 0; proc < end; proc++)
    {
        Process* aProc = &proclist[proc];
        
        if ( aProc->remainingTime!=0 ) {
            
            bool valid = true;
            if ( processQueue.size() > 0 ) { //check current queue for candidate process before adding it to queue
                for(int i = 0; i < processQueue.size(); i++)
                {
                    if ( processQueue[i]->pid == aProc->pid ) {
                        valid = false;
                    }
                }
            }
            if (valid) {
                processQueue.push_back( &proclist[proc] );
            }
        }
        
    }

    // take the last executed process, and put it at the back
    if ( processQueue.size() > 0 ) {
        if ( processQueue.front()->remainingTime != 0 ) {
            processQueue.push_back( processQueue.front() );
        }
        processQueue.pop_front();
    }
}

int executeProcessQueue( std::deque<Process*>& processQueue )
{
    int removedProcesses = 0;

    for(int execs = 0; execs < TQ; execs++)
    {
        removedProcesses += executeProcess( processQueue.front() );
    }

    return removedProcesses;
}

// provided main
int main()
{
    // int arriv[] = {0,1,2,3,4,5};
    // int burst[] = {7,5,3,1,2,1}; 
    // int burst[] = {8,4,2,1,3,2}; // these are from the screenshot
    
    int arriv[] = {0,50,130,190,210,350};
    int burst[] = {250,170,75,100,130,50}; 
    int numberOfProcesses = sizeof arriv / sizeof arriv[0];

    // Generate Process structs
    Process proc[numberOfProcesses] ;

    // timeElapsed represents the amount of time that has elapsed ( 1 iteration of loop = 1 unit of time = elapsed +1)
    timeElapsed = 0;
    // processCount and finishedProcesses represent the end and start bounds (respectively) of the proc array
    int processCount = 0;
    int finishedProcesses = 0;

    std::deque<Process*> processQueue;

    while( finishedProcesses < numberOfProcesses ){
        // check for processes to add, and add them if so.
        processCount += addProcessByTime( proc , processCount , numberOfProcesses, arriv, burst);

        // Build the execution queue
        buildProcessQueue( processQueue , proc , finishedProcesses, processCount);

        // execute the next process
        finishedProcesses += executeProcessQueue( processQueue );
    }

    // sort by finish times so the printed list looks nice
    std::sort( &proc[0], &proc[numberOfProcesses] , sortByPID );
    // std::sort( &proc[0], &proc[numberOfProcesses] , sortByFinishTime );
    // calc avg times.
    findavgTime(proc, numberOfProcesses);
    printf("\n");
    
    return 0;
}
