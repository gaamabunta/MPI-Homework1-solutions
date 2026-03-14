
/*
 * Problem 6 – Part 1: Emulate MPI_Allreduce using MPI_Bcast + MPI_Reduce
 */

#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int local_val, global_sum;

  MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

  local_val = rank;   
    MPI_Reduce(&local_val, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&global_sum, 1, MPI_INT, 0, MPI_COMM_WORLD);

printf("Process %d: global sum = %d\n", rank, global_sum);

    MPI_Finalize();
    return 0;
}