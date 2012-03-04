#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

double  tsFloat (timespec  time)
{
    return ((double) time.tv_sec + (time.tv_nsec / 1000000000.0)) ;

} 

// Implement HyperQuicksort
void Bandwidth()
{
   int rank, size, source, destination, token;
   MPI_Comm_size(MPI_COMM_WORLD, &size);  
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   double start, end;
   int iterations=1000;
   double difList[size];
   MPI_Status status;

   source=0;
   for (int destination = 0; destination < size; destination++)
   {
      //clock_gettime(CLOCK_REALTIME, &start);
      start=MPI_Wtime();
      if (rank == source || rank == destination)
      {
         for (int i = 0; i < iterations; i++)
         {
            // Check if lead processor
            if (rank == source)
            {
               //Broadcast listsize to all processors
               MPI_Send(&token, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
               MPI_Recv(&token, 1, MPI_INT, destination, 0, MPI_COMM_WORLD, &status);
            }
            else if (rank==destination)
            {
               //Recieve listsize from lead processor
               MPI_Recv(&token, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
               MPI_Send(&token, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
            }
         }
         if (rank==source)
         {
            end=MPI_Wtime();
            difList[destination]=end-start;
         }
      }
   }
   if (rank==0)
   {
      cout<<"Timing data is as follows\n";
      for (int i = 0; i < size; i++)
      {
         cout<<"destination "<<i<<" time:" <<difList[i]<<'\n';
      }
   }      
}


int main (int argc, char *argv[])
{

   MPI_Init(&argc, &argv);
   Bandwidth();
   MPI_Finalize();
   return 0;
}