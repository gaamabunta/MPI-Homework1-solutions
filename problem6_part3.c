/*
 * Problem 6 – Part 3: Demonstrate MPI_Alltoall personalized communication
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int *sendbuf, *recvbuf;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sendbuf = (int*)malloc(size * sizeof(int));
    recvbuf = (int*)malloc(size * sizeof(int));

    for (int j = 0; j < size; j++) {
        sendbuf[j] = rank * size + j;
    }
    MPI_Alltoall(sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, MPI_COMM_WORLD);
    printf("Process %d received: [", rank);
    for (int i = 0; i < size; i++) {
        printf("%d%s", recvbuf[i], (i == size-1) ? "]\n" : ", ");
    }

    free(sendbuf);
    free(recvbuf);
    MPI_Finalize();
    return 0;
}