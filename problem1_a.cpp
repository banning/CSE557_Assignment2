// CSE557 Assignment 2
// Problem 1_a
// Calculate cache size and average access latencies
// 03/12/12
// Mike Banning
// Dan Keating

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

void calculateLatency();

int main (int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	// Calculate cache size and average access latencies
	calculateLatency();

	MPI_Finalize();

	return 0;
}

// Calculate cache size and average access latencies
void calculateLatency()
{
	// Intialize variables for testing
	int iInitialSize = 1024;
	int iNumIterations = 10000;
	int iIncrement = 1024;
	int iMaxSize = 100000000;
	char* x[iInitialSize];
	char* y[iInitialSize];
	int iSize = iInitialSize;
	int iDotProd = 0;

	//fill x and y with 1s
	for (int j = 0; j < iMaxSize; j++)
		x[j] = 1;

	for (int k = 0; k < iMaxSize; k++)
		y[k] = 1;

	// Intialize array to save output
	int iOutputSize = 1000;
	int* iTimerStats[iOutputSize];
	iOutputSize = 0;

	// Loop until array is 100,000,000
	while (iSize < iMaxSize)
	{
		//timer start
		for (int i = 0; i < iNumIterations; i++)
		{
			for (int z = 0; z < iSize; z++)
			{
				iDotProd = iDotProd + x[z] * y[z];
			}
		}
		//timer stop	

		//iTimerStats[iOutputSize] = timerOutput;
		iOutputSize++;

		iSize = iSize + iIncrement;
		if (iSize > 1024 && iIncrement == 1024)
			iIncrement = 100 * iIncrement;
	}

	// Output statistics
	for (int m; m < iOutputSize; m++)
		cout <<iTimerStats[m] <<endl;

	return 0;
}