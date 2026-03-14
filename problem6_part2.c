/*
 * Problem 6 – Part 2: Emulate MPI_Allgather using MPI_Gather + MPI_Bcast
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int sendval;
    int *recvbuf = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sendval = rank;

    if (rank == 0) {
        recvbuf = (int*)malloc(size * sizeof(int));
    }

    MPI_Gather(&sendval, 1, MPI_INT, recvbuf, 1, MPI_INT, 0, MPI_COMM_WORLD);

    
    MPI_Bcast(recvbuf, size, MPI_INT, 0, MPI_COMM_WORLD);

    
    printf("Process %d: received array = [", rank);
    for (int i = 0; i < size; i++) {
        printf("%d%s", recvbuf[i], (i == size-1) ? "]\n" : ", ");
    }

    if (rank == 0) free(recvbuf);
    MPI_Finalize();
    return 0;
}