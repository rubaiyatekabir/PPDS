#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int size, rank, token = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank > 0) {
        MPI_Recv(&token, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
        token = 100;

    MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

    for (int i = 0; i < size; i++) {
        if (rank == i) {
            if (rank > 0)
                printf("P%d receives %d from P%d\n", rank, token, rank - 1);

            printf("P%d send %d to P%d\n", rank, token, (rank + 1) % size);
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (rank == 0) {
        MPI_Recv(&token, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P%d receives %d from P%d\n", rank, token, size - 1);
    }

    MPI_Finalize();
    return 0;
}