// CSE557 Assignment 2
// Problem 1_a
// Calculate cache size and average access latencies
// 03/12/12
// Mike Banning
// Dan Keating

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
using namespace std;

void calculateLatency();
double  tsFloat (timespec  time);

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
	int iMaxSize = 10000000;
	int *x = new int[iMaxSize];
	int *y = new int[iMaxSize];
	int iSize = iInitialSize;
	int iDotProd = 0;
	timespec start, end;
	double timerOutput;

	int rank, size;
	MPI_Status status;

	// Find rank and number of processors
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//fill x and y with 1s
	for (int j = 0; j < iMaxSize; j++)
		x[j] = 1;

	for (int k = 0; k < iMaxSize; k++)
		y[k] = 1;

	// Intialize array to save output
	double *iTimerStats = new double[iMaxSize/iIncrement];
	int *iTimerSize = new int[iMaxSize/iIncrement];
	int iOutputSize = 0;

	// Loop until array is 100,000,000
	while (iSize < iMaxSize)
	{
		// Timer start
		//MPI_Barrier(MPI_COMM_WORLD);
		clock_gettime(CLOCK_REALTIME, &start);

		for (int i = 0; i < iNumIterations; i++)
		{
			for (int z = 0; z < iSize; z++)
			{
				iDotProd = iDotProd + x[z] * y[z];
			}
		}

		// Timer stop
		//MPI_Barrier(MPI_COMM_WORLD);
		clock_gettime(CLOCK_REALTIME, &end);
		timerOutput = tsFloat(end)-tsFloat(start);

		// Add timer result to output array
		iTimerStats[iOutputSize] = timerOutput;
		iTimerSize[iOutputSize] = iSize;
		iOutputSize++;

		iSize = iSize + iIncrement;
		if (iSize > 1024 && iIncrement == 1024)
			iIncrement = 100 * iIncrement;
	}

	if (rank == 0)
	{
		// Output statistics
		for (int m = 0; m < iOutputSize; m++)
			cout <<iTimerStats[m] <<"," <<iTimerSize[m] <<endl;
	}
}

double  tsFloat (timespec  time)
{
    return ((double) time.tv_sec + (time.tv_nsec / 1000000000.0)) ;

}