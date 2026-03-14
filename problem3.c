/*
 * Problem 3 – Dot product with tree reduction (by hand)
 * 
 This program computes the dot product of two vectors A and B of length N
 using P processes. The vectors are partitioned as evenly as possible.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1024  

int main(int argc, char *argv[]) {
    int rank, size;
    double *A, *B;
    double local_sum = 0.0, global_sum;
    double start_time, end_time;
    int i, step;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int base = N / size;
    int rem = N % size;
    int local_n = base + (rank < rem ? 1 : 0);
    int start_idx = rank * base + (rank < rem ? rank : rem);

    A = (double*)malloc(local_n * sizeof(double));
    B = (double*)malloc(local_n * sizeof(double));
    if (A == NULL || B == NULL) {
        fprintf(stderr, "Process %d: malloc failed\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

      for (i = 0; i < local_n; i++) {
        A[i] = 1.0;
        B[i] = 1.0;
    }

    for (i = 0; i < local_n; i++) {
        local_sum += A[i] * B[i];
    }

   MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    step = 1;
    while (step < size) {
        if (rank % (2 * step) == 0) {
            if (rank + step < size) {
                double recv_buf;
                MPI_Recv(&recv_buf, 1, MPI_DOUBLE, rank + step, 0,
                         MPI_COMM_WORLD, &status);
                local_sum += recv_buf;
            }
        } else if (rank % (2 * step) == step) {
            MPI_Send(&local_sum, 1, MPI_DOUBLE, rank - step, 0,
                     MPI_COMM_WORLD);
            break;      }
        step *= 2;
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        global_sum = local_sum;
        printf("N = %d, P = %d, dot product = %f, time = %f seconds\n",
               N, size, global_sum, end_time - start_time);
    }

    free(A);
    free(B);
    MPI_Finalize();
    return 0;
}