#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, val, prefix;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("Run with 4 processes.\n");
        MPI_Finalize();
        return 0;
    }

    val = (rank + 1) * 10;

    MPI_Scan(&val, &prefix, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("P%d: Local value = %d, Prefix sum = %d\n", rank, val, prefix);

    int res[4];

    MPI_Gather(&prefix, 1, MPI_INT, res, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nPrefix result: ");
        for (int i = 0; i < 4; i++)
            printf("%d ", res[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
