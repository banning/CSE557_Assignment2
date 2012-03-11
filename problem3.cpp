#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

// Implement HyperQuicksort
void Bandwidth()
{
   int rank, size, source, destination;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Status status;
   int * token = new int[5];
   double start, end;
   int iterations=1;
   int initial_size=1048576;
   int max_size=104857060;
   int increment=1048576;
   int message_size=0;
   int actual_size=0;

   int listsize = (max_size/increment)*(size-1);
   double * difList = new double[listsize];
   int data_save = 0;
   int data_increment = max_size/increment;

   source=0;
   for (int destination = 1; destination < size; destination++)
   {
      MPI_Barrier(MPI_COMM_WORLD);

      if (destination > 1 && rank == 0)
         data_save = destination - 1 + data_increment;

      for (message_size = initial_size; message_size <= max_size; message_size += increment)
      {
         actual_size = message_size/sizeof(MPI_INT);
         delete [] token;
         token = new int[actual_size];
         std::fill_n(token, actual_size, 0);

         start=MPI_Wtime();
         for (int i = 0; i < iterations; i++)
         {
            // Check if lead processor
            if (rank == source)
            {
               MPI_Send(token, actual_size, MPI_INT, destination, 0, MPI_COMM_WORLD);
               MPI_Recv(token, actual_size, MPI_INT, destination, 0, MPI_COMM_WORLD, &status);
            }
            else if (rank == destination)
            {
               MPI_Recv(token, actual_size, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
               MPI_Send(token, actual_size, MPI_INT, source, 0, MPI_COMM_WORLD);
            }
         }
         end=MPI_Wtime();

         if (rank == 0)
         {
            difList[data_save]=end-start;
            data_save += 1;
         }

      }
   }
   
   if (rank==0)
   {
      cout<<"data_increment: " <<data_increment <<endl;
      cout<<"time";
      for (int i = 1; i < listsize; i++)
      {
         cout <<difList[i] <<endl;
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