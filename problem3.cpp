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
   double * outputList = new double[listsize];
   int data_save = 0;
   int data_increment = max_size/increment;

   // Set 0 as lead (source) processor
   source=0;

   // Perform timing test to all processors
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

         // Start timing
         start=MPI_Wtime();
         for (int i = 0; i < iterations; i++)
         {
            // Check if lead processor
            if (rank == source)
            {
               // Send message and wait for acknowledgement
               MPI_Send(token, actual_size, MPI_INT, destination, 0, MPI_COMM_WORLD);
               MPI_Recv(token, actual_size, MPI_INT, destination, 0, MPI_COMM_WORLD, &status);
            }
            else if (rank == destination)
            {
               // Receive message and send acknowledgement
               MPI_Recv(token, actual_size, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
               MPI_Send(token, actual_size, MPI_INT, source, 0, MPI_COMM_WORLD);
            }
         }
         // End timing
         end=MPI_Wtime();

         // Write timing data to outputList
         if (rank == 0)
         {
            outputList[data_save]=end-start;
            data_save += 1;
         }

      }
   }
   
   //Output statistics only if lead processor
   if (rank == source)
   {
      cout<<"data_increment: " <<data_increment <<endl;
      cout<<"time" <<endl;;
      for (int i = 1; i < listsize; i++)
      {
         cout <<outputList[i] <<endl;
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