#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char* argv[])
{
    if (argc != 3) {
	fprintf(stderr, "Usage: %s result.csv time.csv\n", argv[0]);
        return EXIT_FAILURE;
    }

    const int total_points = 1 << 16;  // 65536
    const char* result_file = argv[1];
    const char* time_file   = argv[2];
    int local_count = 0, global_count = 0;
    int points_per_process;


    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    points_per_process = total_points / size;

    // Seed random generator differently for each process
    unsigned int seed = time(NULL) + rank * 12345;
    srand(seed);

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    for (int i = 0; i < points_per_process; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            local_count++;
        }
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (rank == 0) {
        double pi_estimate = 4.0 * ((double)global_count / total_points);
        double elapsed = end_time - start_time;

        FILE* fout = fopen(result_file, "w");
        FILE* ftime = fopen(time_file, "w");

        if (fout) {
            fprintf(fout, "%.8f\n", pi_estimate);
            fclose(fout);
        }

        if (ftime) {
            fprintf(ftime, "%.6f\n", elapsed);
            fclose(ftime);
        }

        printf("Estimated pi = %.8f\n", pi_estimate);
        printf("Time elapsed = %.6f seconds\n", elapsed);
    }

    MPI_Finalize();
    return 0;
}
