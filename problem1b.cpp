// CSE557 Assignment 2
// Problem 1b
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
	// Find rank and number of processors
	int rank, size;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Single threaded
	if (rank == 0)
	{
		// Intialize variables for testing
		int iNumIterations = 10000;
		int iIncrement = 1024;
		int iMaxSize = 5000000;
		int* x = new int[iMaxSize];
		int* y = new int[iMaxSize];
		int iSize = iMaxSize;
		int iDotProd = 0;
		int Ww1, Ww2;
		double baseTimer, start, end;

		//fill x and y with 1s
		for (int j = 0; j < iMaxSize; j++)
			x[j] = 1;

		for (int k = 0; k < iMaxSize; k++)
			y[k] = 1;

		// Intialize array to save output
		double* iTimerStats = new double[iNumIterations];
		double* iTimerAdjusted = new double[iNumIterations];

		// Calculate non-random access time
		// Timer start
		srand ( time(NULL) );
		start = MPI_Wtime();

		iDotProd = iDotProd + x[0] * y[0];

		// Timer stop
		end = MPI_Wtime();

		baseTimer = end - start;

		for (int i = 0; i < iNumIterations; i++)
		{
			iDotProd = 0;

			// Timer start
			srand ( time(NULL) );
			start = MPI_Wtime();

			// Calculate random access time
			for (int z = 0; z < iMaxSize; z++)
			{
				Ww1 = rand()%1 * z;
				Ww2 = rand()%1 * z;
				iDotProd = iDotProd + x[Ww1] * y[Ww2];
			}

			// Timer stop
			end = MPI_Wtime();

			// Add timer result to output array
			iTimerStats[i] = end - start;
			iTimerAdjusted[i] = iTimerStats[i] - baseTimer;
		}

		// Output statistics
		cout <<"Timer,Adjusted,Size" <<endl;
		cout<<"0," <<baseTimer <<"," <<iSize <<endl;
		for (int m = 0; m < iNumIterations; m++)
			cout <<iTimerStats[m] <<"," <<iTimerAdjusted[m] <<"," <<iSize <<endl;
	}
}