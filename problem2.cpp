// CSE557 Assignment 2
// Problem 2
// Calculate commuication latency
// 03/12/12
// Mike Banning
// Dan Keating

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main (int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	// Calculate commuication latency

	MPI_Finalize();
	
	return 0;
}