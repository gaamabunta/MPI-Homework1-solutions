/*
 * Problem 4 – Dot product with tree reduction (LLM‑generated)
 * 
 * This code was produced by ChatGPT‑4 (March 2025) after a few iterations.
 * It follows the same logic as Problem 3.
 * Prompt:
Write an MPI program in C that computes the dot product of two vectors using a tree reduction with only point‑to‑point communication.
The vectors are of length N, and there are P processes. The program should partition the data evenly, use MPI_Wtime to measure the total time, and print the result and time from process 0. Use easy test data (e.g., all ones).
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
    int start = rank * base + (rank < rem ? rank : rem);

    A = (double*)malloc(local_n * sizeof(double));
    B = (double*)malloc(local_n * sizeof(double));

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
                double buf;
                MPI_Recv(&buf, 1, MPI_DOUBLE, rank + step, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_sum += buf;
            }
        } else if (rank % (2 * step) == step) {
            MPI_Send(&local_sum, 1, MPI_DOUBLE, rank - step, 0,
                     MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    end_time = MPI_Wtime();

    if (rank == 0) {
        global_sum = local_sum;
        printf("N = %d, P = %d, dot = %f, time = %f seconds\n",
               N, size, global_sum, end_time - start_time);
    }

    free(A);
    free(B);
    MPI_Finalize();
    return 0;
}