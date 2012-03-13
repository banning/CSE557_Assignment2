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
   int * token = new int(5);
   double start, end;
   int iterations=10;
   MPI_Request request;
   MPI_Status status;
   int initial_size=1048765;
   int max_size=10487650;
   int increment=1048765;
   double difList[(max_size-initial_size)/increment][size];
   source=0;
   //Runs through all possible destinations
   for (int destination = 0; destination < size; destination++)
   {
      if (rank == source || rank == destination)
      {
         //Runs though the set of sizes
         for (int m_size=initial_size; m_size<=max_size; m_size= m_size + increment)
         {
            //rescales token
            int token[m_size/sizeof(MPI_INT)];
            start=MPI_Wtime();
            for (int i = 0; i < iterations; i++)
            {
               // Check if lead processor
               if (rank == source)
               {
                  //Broadcast listsize to all processors
                  MPI_Send(token, m_size, MPI_INT, destination, 0, MPI_COMM_WORLD);
                  MPI_Recv(token, m_size, MPI_INT, destination, 0, MPI_COMM_WORLD, &status);
               }
               else if (rank==destination)
               {

                  //Recieve listsize from lead processor

                  MPI_Recv(token, m_size, MPI_INT, source, 0, MPI_COMM_WORLD, &status);
                  MPI_Send(token, m_size, MPI_INT, source, 0, MPI_COMM_WORLD);
               }
            }
            if (rank==0)
            {  
               end=MPI_Wtime();
               difList[(m_size-initial_size)/increment][destination]=end-start;
            }
         }
      }
   }
   if (rank==0)
   {
      cout<<"Timing data is as follows\n";
      for (int i = 0; i < size; i++)
      {
         cout<<"destination "<<i<<'\n';
         for (int j = 0; j <= (max_size-initial_size)/increment; j++)
         {
            cout<<"Size: " << j*increment <<"\tTime: "<< difList[j][i]<<endl;
         }
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