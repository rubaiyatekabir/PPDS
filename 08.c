#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, send, recv;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    send = (rank == 0) ? 100 : 200;

    // Unsafe version: Potential deadlock
    /*
    MPI_Send(&send, 1, MPI_INT, 1-rank, 0, MPI_COMM_WORLD);
    MPI_Recv(&recv, 1, MPI_INT, 1-rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    */

    // Corrected version
    if (rank == 0) {
        MPI_Send(&send, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("P0 sent %d\n", send);

        MPI_Recv(&recv, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P0 received %d\n", recv);
    }
    else {
        MPI_Recv(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("P1 received %d\n", recv);

        MPI_Send(&send, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("P1 sent %d\n", send);
    }

    if (rank == 0)
        printf("Communication completed without deadlock.\n");

    MPI_Finalize();
    return 0;
}
