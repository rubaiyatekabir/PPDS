#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *sendbuf = malloc(size * sizeof(int));
    int *recvbuf = malloc(size * sizeof(int));

    for (int j = 0; j < size; j++)
        sendbuf[j] = rank * 10 + j;

    MPI_Barrier(MPI_COMM_WORLD);
    double t1 = MPI_Wtime();

    MPI_Alltoall(sendbuf, 1, MPI_INT, recvbuf, 1, MPI_INT, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - t1;

    if (rank == 0)
        printf("After MPI_Alltoall:\n\n");

    printf("P%d received: ", rank);
    for (int i = 0; i < size; i++)
        printf("%d ", recvbuf[i]);

    printf("| Time: %f s\n", elapsed);

    free(sendbuf);
    free(recvbuf);

    MPI_Finalize();
    return 0;
}
