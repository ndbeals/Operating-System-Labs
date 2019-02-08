#include <iostream>

using namespace std;

// Process struct defn'
struct Process
{
    int pid; // Process ID
    int burstTime; // Burst Time
};

void findWaitingTime(Process proc[], int numberOfProcesses, int waitTime[])
{
    // waiting time for first process is 0
    waitTime[0] = 0;

    // calculating waiting time, loop through all processes, add the lastproc waitime + lastproc burst time to get current proc wait time.
    for(int idx = 1; idx < numberOfProcesses; idx++)
    {
        // lastProc variable to make it more readable
        Process lastProc = proc[idx - 1];
        // do the math
        waitTime[ idx ] = lastProc.burstTime + waitTime[ idx - 1 ];
    }
}

void findTurnaroundTime(Process proc[], int numberOfProcesses, int waitTime[], int turnaroundTime[])
{
    // wait time for first process is just the burst time.
    turnaroundTime[0] = proc[0].burstTime;

    // loop through all the other processes, calculate burst time by adding the waittime of the proc + the burst time of the proc.
    for(int idx = 1; idx < numberOfProcesses; idx++)
    {
        // curProc variable to make code more readable
        Process curProc = proc[ idx ];
        // do the math
        turnaroundTime[ idx ] = waitTime[ idx ] + curProc.burstTime;
    }
}

void displayProcessList(Process proc[], int numberOfProcesses, int waitTime [], int turnaroundTime [])
{
    // Write code to Display processes along with all details (Process ID, Burst Time, Waiting Time, Turnaround Time)
    printf("\nProcesses   Burst Time   Waiting Time   Turn around Time\n");
    // loop through all the processes, printing the information we've calculated about them.
    for(int idx = 0; idx < numberOfProcesses; idx++)
    {
        printf(" %d \t\t %d \t\t %d \t\t %d\n" , proc[idx].pid , proc[idx].burstTime , waitTime[idx] , turnaroundTime[idx] );
    }
}

void findavgTime(Process proc[], int numberOfProcesses)
{
    int waitTime[numberOfProcesses], turnaroundTime[numberOfProcesses];

    // Your function to find waiting time of all processes
    findWaitingTime(proc, numberOfProcesses, waitTime);
    // Your function to find turnaround time for all processes
    findTurnaroundTime(proc, numberOfProcesses, waitTime, turnaroundTime);
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
    avgWaitTime /= 5;
    avgTurnTime /= 5;
    
    printf("Average waiting time \t= %.1f ms\n",avgWaitTime);
    printf("Average turnaround time = %.1f ms\n",avgTurnTime);
    
}

void sorting(Process proc[] , int numberOfProcesses)
{
    // Implement a basic sort algorithm which sorts only on the Burst Time value.
    // insertion sort
    // define temporary and loop index values;
    int sortIdx, cmpIdx;
    Process tmpVal;
    for(int sortIdx = 1; sortIdx < numberOfProcesses; sortIdx++)
    {
        // start comparing all elements between 1 and sortIdx.
        cmpIdx = sortIdx;
        
        // sort between 1 to sortIdx, decrementing each time an insertion is made.
        while( (cmpIdx > 0 ) && ( proc[ cmpIdx - 1 ].burstTime > proc[ cmpIdx ].burstTime ) ){
            tmpVal = proc[ cmpIdx ];
            proc[ cmpIdx ] = proc[ cmpIdx - 1 ];
            proc[ cmpIdx - 1 ] = tmpVal;
            cmpIdx--;
        }
    }
}

// provided main
int main()
{
    Process proc[] = {{1, 6}, {2, 8}, {3, 7}, {4, 3}, {5,4}};
    int numberOfProcesses = sizeof proc / sizeof proc[0];

    // sort our processes.
    sorting(proc, numberOfProcesses);

    cout << "\nOrder in which process gets executed\n";
    for (int i = 0 ; i < numberOfProcesses; i++)
        cout << proc[i].pid <<" ";

    // calc avg times.
    findavgTime(proc, numberOfProcesses);
    return 0;
}
