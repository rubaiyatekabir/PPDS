#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int size, rank, sum = 0, ans = 0;
    int mat[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    int local[4];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&mat[i], 4, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < 4; i++)
            sum += mat[0][i];

        printf("P%d sum = %d\n", rank, sum);
    }

    if (rank > 0) {
        MPI_Recv(&local, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < 4; i++)
            sum += local[i];

        printf("P%d sum = %d\n", rank, sum);
    }

    MPI_Reduce(&sum, &ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("%d\n", ans);

    MPI_Finalize();
}
