// CSE557 Assignment 2
// Problem 1_b
// Calculate random access latency
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

	// Calculate random access latency
	calculateLatency();

	MPI_Finalize();

	return 0;
}

// Calculate random access latency
void calculateLatency()
{
	// Intialize variables for testing
	int iInitialSize = 100000000;
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
			for (int z = 0; z < iInitialSize; z++)
			{
				//Ww1 = random_number(0 to 1) * iSize
				//Ww2 = random_number(0 to 1) * iSize
				//iDotProd = iDotProd + x[Ww1] * y[Ww2];
			}
		}
		//timer stop	

		//iTimerStats[iOutputSize] = timerOutput;
		iOutputSize++;

		// iSize = iSize + iIncrement;
		// if (iSize > 1024 && iIncrement == 1024)
		// 	iIncrement = 100 * iIncrement;
	}

	// Output statistics
	for (int m; m < iOutputSize; m++)
		cout <<iTimerStats[m] <<endl;

	return 0;
}