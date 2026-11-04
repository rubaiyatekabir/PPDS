#include <stdio.h>
#include <mpi.h>

#define DATA_SIZE 16
#define NUM_BINS 10

int main(int argc, char *argv[]) {
    int rank, size;
    int dataset[DATA_SIZE] = {
        1, 2, 2, 3,
        2, 3, 4, 4,
        1, 4, 5, 5,
        0, 2, 5, 9
    };

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = DATA_SIZE / size;
    int local_data[local_n];
    int local_hist[NUM_BINS] = {0};
    int global_hist[NUM_BINS] = {0};

    MPI_Scatter(dataset, local_n, MPI_INT, local_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++)
        local_hist[local_data[i]]++;

    MPI_Allreduce(local_hist, global_hist, NUM_BINS, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Final histogram after MPI_Allreduce:\n\n");
        printf("Value    Frequency\n");

        for (int i = 0; i < NUM_BINS; i++)
            printf("%-9d%d\n", i, global_hist[i]);

        printf("\nTotal number of elements = %d\n", DATA_SIZE);
    }

    MPI_Finalize();
    return 0;
}
