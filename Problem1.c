/*
 * Problem 1 – Ring of processes (by hand)
 * 
 */

#include <stdio.h>
#include <mpi.h>

#define N 5   

int main(int argc, char *argv[]) {
    int rank, size;
     int token;
   int next, prev;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (size < 2) {
        if (rank == 0)
            fprintf(stderr, "Error: At least 2 processes required.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
 next = (rank + 1) % size;
    prev = (rank - 1 + size) % size;

 if (rank == 0) {
        token = 0;   
      for (i = 0; i < N; i++) {
         
         MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
                MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        token++;

            printf("Process 0 completed round %d, token = %d\n", i+1, token);
        }

      printf("Final token after %d trips = %d\n", N, token);

    } else {
        for (i = 0; i < N; i++) {
          MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         token++;
           printf("Process %d received token, now = %d\n", rank, token);
       MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        }
   }

   MPI_Finalize();
    return 0;
}