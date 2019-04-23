#include <iostream>
#include <stdio.h>
#include <string>
#include <numeric>
#include <algorithm>

// supplied values
int blockSize[] = {5,10,50,100};
int numBlocks = sizeof blockSize / sizeof blockSize[0];
int processSize[] = {11,6,34,5,25,60};
int numberOfProcesses = sizeof processSize / sizeof processSize[0];

// process struct
struct Process
{
	int pid;
	int blockNumberFF;
	int blockNumberNF;
	int size;
};


int main()
{
	// create an array to hold our processes
	Process* list[numberOfProcesses];

	// populate array
	for(int i = 0; i < numberOfProcesses; i++)
	{
		list[i] = new Process{ i+1 , 0 , 0 , processSize[i] };
	}

	// First First
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		Process* proc = list[pid];
		for(int block = 0; block < numBlocks; block++)
		{
			// if the process size is less than or equal to block size, assign it to this block
			if (proc->size <= blockSize[block] ) {
				proc->blockNumberFF = block + 1;

				// subtract size from remaining
				blockSize[block] -= proc->size;

				//exit loop
				break;
			}
		}
	}

	int blockSize[] = {5,10,50,100};
	
	// Best Fit
	int lastblock = 0;
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		for(int block = lastblock; block < numBlocks; block++)
		{
			Process* proc = list[pid];
			// if the process size is less than or equal to block size, assign it to this block
			if (proc->size <= blockSize[block] && proc->blockNumberNF < 1 ) {
				proc->blockNumberNF = block + 1;

				// subtract size from remaining
				blockSize[block] -= proc->size;

				lastblock=block;
			}
		}
	}
	

	// print processes
	printf("First Fit:\nPID   Process Size   Block no.\n");
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		Process* proc = list[pid];
		printf(" %d         %2d            %d\n",proc->pid,proc->size,proc->blockNumberFF);
	}	


	// print processes
	printf("Next Fit:\nPID   Process Size   Block no.\n");
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		Process* proc = list[pid];
		if (proc->blockNumberNF==0) {
			printf(" %d        %2d            %s\n",proc->pid,proc->size,"Not Allocated");
		}
		else
		{
			printf(" %d        %2d            %d\n",proc->pid,proc->size,proc->blockNumberNF);
		}
	}

}