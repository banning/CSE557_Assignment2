#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
using namespace std;

double tsFloat (timespec time)
{
    return ((double) time.tv_sec + (time.tv_nsec / 1000000000.0)) ;

}

// Implement HyperQuicksort
void Bandwidth()
{
   int rank, size, source, destination;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   int * token = new int(5);
   double start, end;
   int iterations=1;
   double difList[size];
   MPI_Status status;
   int initial_size=1048576;
   int max_size=104857060;
   int increment=1048576;

   source=0;
   for (int destination = 0; destination < size; destination++)
   {
      MPI_Barrier(MPI_COMM_WORLD);
      if (rank == source || rank == destination)
      {
         for (int size=initial_size; size<=max_size; size= size + increment)
         {
            token = new int(size/sizeof(MPI_INT));
            start=MPI_Wtime();
            for (int i = 0; i < iterations; i++)
            {
               // Check if lead processor
               if (rank == source)
               {
                  //Broadcast listsize to all processors
                  cout<<rank << " sending"<<endl;
                  MPI_Send(&token, size, MPI_INT, destination, 0, MPI_COMM_WORLD);
                  MPI_Recv(&token, size, MPI_INT, destination, 0, MPI_COMM_WORLD, &status);
               }
               else if (rank==destination)
               {

                  //Recieve listsize from lead processor
                  MPI_Recv(&token, size, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
                  cout<<rank << " received"<<endl;
                  MPI_Send(&token, size, MPI_INT, source, 0, MPI_COMM_WORLD);
               }
            }
         }
         /*
         if (rank==source)
         {
            end=MPI_Wtime();
            difList[destination]=end-start;
         }*/
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