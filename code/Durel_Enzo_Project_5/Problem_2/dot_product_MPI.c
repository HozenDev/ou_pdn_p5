#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

double compute_dot_product(double *a, double *b, int n) {
    double result = 0.0;
    for (int i = 0; i < n; i++)
        result += (double)a[i] * b[i];
    return result;
}

void read_csv(const char* filename, double* data, int n) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++)
        fscanf(file, "%lg,", &data[i]);  // assuming CSV format
    fclose(file);
}

int main(int argc, char** argv) {
    int rank, size;
    int n;

    if (argc != 6) {
        fprintf(stderr, "Usage: %s n vec1.csv vec2.csv result.csv time.csv\n", argv[0]);
        return EXIT_FAILURE;
    }

    n = atoi(argv[1]);
    const char* file_vec1 = argv[2];
    const char* file_vec2 = argv[3];
    const char* file_result = argv[4];
    const char* file_time = argv[5];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_n = n / size;
    double *vec1_local = (double*) malloc(local_n * sizeof(double));
    double *vec2_local = (double*) malloc(local_n * sizeof(double));

    double *vec1 = NULL;
    double *vec2 = NULL;

    if (rank == 0) {
        vec1 = (double*) malloc(n * sizeof(double));
        vec2 = (double*) malloc(n * sizeof(double));

        read_csv(file_vec1, vec1, n);
        read_csv(file_vec2, vec2, n);
    }

    MPI_Barrier(MPI_COMM_WORLD); // Synchronize before timing
    double start_time = MPI_Wtime();

    MPI_Scatter(vec1, local_n, MPI_DOUBLE, vec1_local, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vec2, local_n, MPI_DOUBLE, vec2_local, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double local_result = compute_dot_product(vec1_local, vec2_local, local_n);

    double global_result = 0.0;
    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;

    if (rank == 0) {
        FILE* f_out = fopen(file_result, "w");
        FILE* f_time = fopen(file_time, "w");
        if (f_out) {
            fprintf(f_out, "%.6f\n", global_result);
            fclose(f_out);
        }
        if (f_time) {
            fprintf(f_time, "%.6f\n", elapsed);
            fclose(f_time);
        }
    }

    if (rank == 0) {
        free(vec1);
        free(vec2);
    }

    free(vec1_local);
    free(vec2_local);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
