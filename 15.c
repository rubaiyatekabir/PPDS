#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size, n = 100;
    double a = 0.0, b = 1.0, h = (b - a) / n, local_sum = 0.0, total;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int start = rank * (n / size);
    int end = (rank + 1) * (n / size);

    for (int i = start; i < end; i++) {
        double x1 = a + i * h, x2 = a + (i + 1) * h;
        local_sum += (x1 * x1 + x2 * x2) * h / 2.0;
    }

    MPI_Reduce(&local_sum, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double exact = 1.0 / 3.0;
        printf("Numerical integral ≈ %f\n", total);
        printf("Exact integral = %f\n", exact);
        printf("Absolute error ≈ %f\n", fabs(total - exact));
    }

    MPI_Finalize();
    return 0;
}
