// CSE557 Assignment 2
// Problem 1a
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
	// Find rank and number of processors
	int rank, size;
	MPI_Status status;
	
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// Single threaded
	if (rank == 0)
	{

		// Intialize variables for testing
		int iInitialSize = 1024;
		int iNumIterations = 10000;
		int iIncrement = 1024;
		int iMaxSize = 5000000;
		int *x = new int[iMaxSize];
		int *y = new int[iMaxSize];
		int iSize = iInitialSize;
		int iDotProd = 0;
		double start, end;

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
			start = MPI_Wtime();

			for (int i = 0; i < iNumIterations; i++)
			{
				for (int z = 0; z < iSize; z++)
				{
					iDotProd = iDotProd + x[z] * y[z];
				}
			}

			// Timer stop
			//MPI_Barrier(MPI_COMM_WORLD);
			end = MPI_Wtime();

			// Add timer result to output array
			iTimerStats[iOutputSize] = end - start;
			iTimerSize[iOutputSize] = iSize;
			iOutputSize++;

			iSize = iSize + iIncrement;

			// Implement increase in iIncrement if size greater than 1mb
			if (iIncrement == 1024 && sizeof(int)*iSize > 1048576 )
				iIncrement = 100 * iIncrement;
		}

		// Output statistics
		cout <<"Timer,Size" <<endl;
		for (int m = 0; m < iOutputSize; m++)
			cout <<iTimerStats[m] <<"," <<iTimerSize[m] <<endl;
	}
}