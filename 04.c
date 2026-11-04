#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void run_ping_pong(int size_bytes, int iters, int rank) {
    char *buf = (char *)malloc(size_bytes);
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < iters; i++) {
        if (rank == 0) {
            MPI_Send(buf, size_bytes, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(buf, size_bytes, MPI_BYTE, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rank == 1) {
            MPI_Recv(buf, size_bytes, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buf, size_bytes, MPI_BYTE, 0, 1, MPI_COMM_WORLD);
        }
    }

    double elapsed = MPI_Wtime() - start;
    if (rank == 0) {
        double avg_rt = elapsed / iters;
        double avg_ow = avg_rt / 2.0;
        double bw_mbps = (size_bytes / avg_ow) / 1e6;

        printf("\nMessage size : %d bytes\n", size_bytes);
        printf("Iterations : %d\n", iters);
        printf("Total round trips : %d\n", iters);
        printf("Average round-trip : %.6f seconds\n", avg_rt);
        printf("Average one-way : %.6f seconds\n", avg_ow);
        printf("Bandwidth : %.2f MB/s\n", bw_mbps);
    }
    free(buf);
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) printf("Run with exactly 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    int sizes[] = {64, 256, 1024, 4096, 16384, 65536, 1048576};
    int num_tests = sizeof(sizes) / sizeof(sizes[0]);
    int iters = 5;

    for (int i = 0; i < num_tests; i++) {
        run_ping_pong(sizes[i], iters, rank);
    }

    MPI_Finalize();
    return 0;
}
