#include <iostream>
#include <algorithm>

// Number of processes 
const int P = 5; 
// Number of resources 
const int R = 3;


// Calculate the need matrix based on the max and allocated matrices
int (*calculateNeedMatrix( int (*allocated)[R] , int (*maximum)[R] ))[R]
{
    // initialize the return value to be the same dimensions as allocated or maximum
    int (*ret)[R] = new int[P][R];

    // Loop over all proceses
    for(int proc = 0; proc < P; proc++)
    {
        // loop over all resources in process
        for(int res = 0; res < R; res++)
        {
            // calculate the need
            ret[ proc ][res] = maximum[ proc ][ res ] - allocated[ proc ][ res ];
        }
    }
    // return our need
    return ret;
}


// Add corresponding values from need to available
void addResourceArray( int* need, int* available )
{
    // loop over resource amount
    for(int res = 0; res < R; res++)
    {
        // sum appropriate fields
        available[res] += need[res];
    }
}

// compare an available array to a need array
bool compareResourceArray( int* need , int* available )
{
    // initialize success to true, this means if even one thing fails, success will be false
    bool success = true;

    // loop over the resources
    for(int res = 0; res < R; res++)
    {
        // compare if the available can satisfy the need
        if ( need[res] > available[res] ) {
            success = false;
        }
    }
    return success;
}

// find the safe execution sequence, given a need matrix and available array, outputting results to sequence
bool findSafeSequence( int (*need)[R] , int (available)[R] , int* sequence) 
{
    // loop need array to check if there are any processes we can execute
    bool success = false;
    int safeSeqIdx = 0;
    bool finished[P];
    // finish initializing default vals

    do {
        // reset success to false each time before we loop through the process list again
        success = false;

        // loop through processes
        for(int proc = 0; proc < P; proc++)
        {
            // check if current process can be satisfied (ran) with the current available resources
            bool canRun = compareResourceArray( need[proc] , available );
            // printf(" test %d  less than: %s\n",sequence[proc],(sequence[proc] < 0)?"true":"false") ;
            if ( canRun && !finished[proc] ) {
                success = true;

                // add proc num + 1 to array (kind of a hack to keep track of which processes we've handled. just subtract 1 later)
                sequence[ safeSeqIdx ] = proc;
                safeSeqIdx++;
                finished[ proc ] = true;

                addResourceArray( need[proc] , available );

                // printf("proc: %d safeseqidx: %d canRun: %s , res arr: %d,%d,%d\n",proc,safeSeqIdx,canRun?"true":"false",available[0],available[1],available[2]);
            }
            
        } 
        // exit(0);
        // break;
    }
    while(safeSeqIdx < P && success);
    

    return success;
}

int main()
{

    //Total processes 
    int processes[P] = {0, 1, 2, 3, 4}; 
    // Available instances of resources 
    int available[R] = {3, 3, 2};
    // Maximum R that can be allocated to processes     
    int maximum[P][R] = {
        {7, 3, 5},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}};
    // Resources allocated to processes     
    int allocated[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}};

    int (*need)[R] = calculateNeedMatrix( allocated , maximum);
    

    printf( "Need Matrix: \n");
    for(int i = 0; i < P; i++)
    {
        printf("  [%d,%d,%d]\n",need[i][0],need[i][1],need[i][2]);
    }

    int safeSequence[P];
    std::fill( &safeSequence[0] , &safeSequence[P] , -1 );
    bool isSafeState = findSafeSequence( need , available , safeSequence);

    if (isSafeState) {
        printf("\nSystem is in a safe state.\nSafe sequence is: ");

        for(int seq = 0; seq < P; seq++)
        {
            // printf("is safe: %s safe seq idx 0: %d \n",isSafeState?"True":"False",safeSequence[seq]);
            if ( seq < (P-1) ) {
                printf( "P%d, " , safeSequence[ seq ] );
            }
            else
            {
                printf( "P%d\n" , safeSequence[ seq ] );
            }
        }
    }
    else
    {
        printf("System is in an unsafe state.\n");
    }
    

    

    

    // printf("hello world!\n");
    return 0;
}