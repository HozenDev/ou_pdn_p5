#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ITERATIONS 1000

int main(int argc, char** argv) {
    int rank, size;
    int n_items;
    int *array = NULL;
    double start, end, total_time;
    FILE *fp;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s n_items output_csv_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    n_items = atoi(argv[1]);
    char* filename = argv[2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) fprintf(stderr, "Error: This program must be run with 2 processes.\n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    array = (int*)malloc(sizeof(int) * n_items);
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Initialize array in rank 0
    if (rank == 0) {
        for (int i = 0; i < n_items; i++) {
            array[i] = rand();
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); // Ensure both start at same time
    start = MPI_Wtime();

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        if ((i + rank) % 2 == 0) {
            // send
            MPI_Send(array, n_items, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD);
        } else {
            // recv
            MPI_Recv(array, n_items, MPI_INT, 1 - rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    total_time = end - start;

    // Only rank 0 writes to file
    if (rank == 0) {
        fp = fopen(filename, "w");
        if (fp == NULL) {
            fprintf(stderr, "Unable to open file %s\n", filename);
        } else {
            double avg_one_way_time = total_time / (NUM_ITERATIONS * 2); // back and forth = 2 one-ways
            fprintf(fp, "n_items,avg_one_way_time_seconds\n");
            fprintf(fp, "%d,%.10f\n", n_items, avg_one_way_time);
            fclose(fp);
        }
    }

    free(array);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
