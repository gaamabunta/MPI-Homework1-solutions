/*
 * Problem 5 – MPI_Comm_split with two different colors (by hand)
 * This program splits MPI_COMM_WORLD into two different sets of
 * sub‑communicators:
 *   1) color = rank / Q   
 *   2) color = rank % Q   
 */

#include <stdio.h>
#include <mpi.h>

#define P 2   
#define Q 4   

int main(int argc, char *argv[]) {
    int rank, size;
    int color_div, color_mod;
    MPI_Comm comm_div, comm_mod;
    int new_rank_div, new_rank_mod;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
     MPI_Comm_size(MPI_COMM_WORLD, &size);

   
    if (size != P * Q) {
       if (rank == 0)
       fprintf(stderr, "Error: world size must be %d (P*Q).\n", P * Q);
       MPI_Abort(MPI_COMM_WORLD, 1);
   }

    
    color_div = rank / Q;
    MPI_Comm_split(MPI_COMM_WORLD, color_div, rank, &comm_div);

   
   color_mod = rank % Q;
   MPI_Comm_split(MPI_COMM_WORLD, color_mod, rank, &comm_mod);

 MPI_Comm_rank(comm_div, &new_rank_div);
 MPI_Comm_rank(comm_mod, &new_rank_mod);

    printf("Global rank %2d: div color=%d new rank=%d | mod color=%d new rank=%d\n",
           rank, color_div, new_rank_div, color_mod, new_rank_mod);

  MPI_Comm_free(&comm_div);
  MPI_Comm_free(&comm_mod);
  MPI_Finalize();
    return 0;
}