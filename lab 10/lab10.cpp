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
	int blockNumberBF;
	int blockNumberWF;
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

	// Best Fit
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		Process* proc = list[pid];
		int rem=1000000;
		int blockID=-1;

		for(int block = 0; block < numBlocks; block++)
		{
			if ((blockSize[block] - proc->size) < rem && (blockSize[block]-proc->size)>=0 && rem!=0 ) {
				rem = (blockSize[block]-proc->size);
				blockID = block;
			}
		}

		proc->blockNumberBF = blockID+1;
		blockSize[blockID] -= proc->size;
	}

	int blockSize[] = {5,10,50,100};
	
	// Worst Fit
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		Process* proc = list[pid];
		int rem=-1000000;
		int blockID=-1;

		for(int block = 0; block < numBlocks; block++)
		{
			if ((blockSize[block] - proc->size) > rem && (blockSize[block]-proc->size)>=0 ) {
				rem = (blockSize[block]-proc->size);
				blockID = block;
			}
		}

		proc->blockNumberWF = blockID+1;
		blockSize[blockID] -= proc->size;
	}
	

	// print processes
	printf("Best Fit:\nPID   Process Size   Block no.\n");
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		Process* proc = list[pid];
		printf(" %d         %2d            %d\n",proc->pid,proc->size,proc->blockNumberBF);
	}	


	// print processes
	printf("Worst Fit:\nPID   Process Size   Block no.\n");
	for(int pid = 0; pid < numberOfProcesses; pid++)
	{
		Process* proc = list[pid];
		if (proc->blockNumberWF==0) {
			printf(" %d        %2d            %s\n",proc->pid,proc->size,"Not Allocated");
		}
		else
		{
			printf(" %d        %2d            %d\n",proc->pid,proc->size,proc->blockNumberWF);
		}
	}

}