#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, params[3], result;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        params[0] = 1000;
        params[1] = 50;
        params[2] = 256;
    }

    MPI_Bcast(params, 3, MPI_INT, 0, MPI_COMM_WORLD);

    result = 0;
    for (int i = 0; i < params[0]; i++) {
        if (i % params[2] < params[1])
            result++;
    }

    printf("P%d: Iterations=%d, Threshold=%d, Block=%d, Result=%d\n",
           rank, params[0], params[1], params[2], result);

    MPI_Finalize();
    return 0;
}
