#include <mpi.h>
#include <stdio.h>
#define N 8

int main(int argc, char *argv[]) {
    int rank, a[N], recv[N];
    MPI_Request req[2];
    double t1, blocking, nonblocking;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (int i = 0; i < N; i++)
        a[i] = (rank == 0) ? (i + 1) * 10 : i + 1;

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    if (rank == 0) {
        MPI_Send(a, N, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(recv, N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else {
        MPI_Recv(recv, N, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(a, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    blocking = MPI_Wtime() - t1;

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    MPI_Irecv(recv, N, MPI_INT, 1 - rank, 1, MPI_COMM_WORLD, &req[0]);
    MPI_Isend(a, N, MPI_INT, 1 - rank, 1, MPI_COMM_WORLD, &req[1]);
    MPI_Waitall(2, req, MPI_STATUSES_IGNORE);

    nonblocking = MPI_Wtime() - t1;

    printf("P%d received: ", rank);
    for (int i = 0; i < N; i++)
        printf("%d ", recv[i]);
    printf("\n");

    if (rank == 0)
        printf("Blocking time    = %f s\nNon-blocking time = %f s\n", blocking, nonblocking);

    MPI_Finalize();
    return 0;
}
