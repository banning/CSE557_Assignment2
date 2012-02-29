// CSE557 Assignment 2
// Problem 1_b
// Calculate random access latency
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

	// Calculate random access latency
	calculateLatency();

	MPI_Finalize();

	return 0;
}

// Calculate random access latency
void calculateLatency()
{
	// Intialize variables for testing
	int iNumIterations = 10000;
	int iIncrement = 1024;
	int iMaxSize = 10000000;
	int* x = new int[iMaxSize];
	int* y = new int[iMaxSize];
	int iSize = iMaxSize;
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
	double* iTimerStats = new double[iMaxSize/iIncrement];
	int iOutputSize = 0;

	for (int i = 0; i < iNumIterations; i++)
	{
		// Timer start
		//MPI_Barrier(MPI_COMM_WORLD);
		clock_gettime(CLOCK_REALTIME, &start);

		for (int z = 0; z < iMaxSize; z++)
		{
			//Ww1 = random_number(0 to 1) * iSize
			//Ww2 = random_number(0 to 1) * iSize
			//iDotProd = iDotProd + &x[Ww1] * &y[Ww2];
		}

		// Timer stop
		//MPI_Barrier(MPI_COMM_WORLD);
		clock_gettime(CLOCK_REALTIME, &end);
		timerOutput = tsFloat(end)-tsFloat(start);

		// Add timer result to output array
		iTimerStats[iOutputSize] = timerOutput;
		iOutputSize++;
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