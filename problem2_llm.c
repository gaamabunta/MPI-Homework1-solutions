/**openAI generated code for a ring communication pattern using MPI in C. 
 * Each process sends a message to the next process in the ring, and the message circulates around the ring a specified number of times. 
 * The value is incremented each time it passes through a process, and the output shows the value at each step.
 * Hand‑written used C++ (iostream, cout); LLM used C (stdio.h, printf).
 * LLM prints each receive on non‑zero processes; hand‑written only printed from process 0.
 * 
 * Prompt: "Write an MPI program in C that passes an integer around a logical ring of processes. 
 *          The integer starts at 0 in process 0 and is incremented each time it passes a process. 
 *          The message should go around the ring N times, where N is defined at compile time."**/



#include <stdio.h>
#include <mpi.h>

#define N 5   /* number of full trips */

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
            printf("Error: at least 2 processes are needed.\n");
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
            printf("Round %d completed by process 0, token = %d\n", i+1, token);
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