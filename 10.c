#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, item;
    int data[] = {10, 20, 30, 40, 50, 60, 70, 80};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 0; i < 8; i++) {
            printf("P0 produced: %d\n", data[i]);
            fflush(stdout);
            MPI_Send(&data[i], 1, MPI_INT, (i % 2) + 1, 0, MPI_COMM_WORLD);
        }

        item = -1;
        MPI_Send(&item, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&item, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

        printf("All 8 items processed successfully.\n");
    }
    else {
        while (1) {
            MPI_Recv(&item, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (item == -1)
                break;

            printf("P%d consumed: %d\n", rank, item);
            fflush(stdout);
        }
    }

    MPI_Finalize();
    return 0;
}
