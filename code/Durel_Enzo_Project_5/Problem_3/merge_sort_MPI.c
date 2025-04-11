#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"

#define COMPARE(a, b) (((a) > (b)) - ((a) < (b)))
#define MAXLINE 25
#define DEBUG   0

float* read_input(FILE* inputFile, int n_items);
int cmpfloat(const void* a, const void* b);
float* merge_arrays(float* a, int a_len, float* b, int b_len);

/* Main Program */
int main (int argc, char *argv[])
{
    if (argc != 5) {
        printf("USE LIKE THIS: merge_sort_MPI n_items input.csv output.csv time.csv\n");
        return EXIT_FAILURE;
    }

    FILE* inputFile = fopen(argv[2], "r");
    int  n_items = strtol(argv[1], NULL, 10);

    int my_rank, comm_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    float* global_array = NULL;

    // Compute scatter counts
    int *counts = malloc(comm_size * sizeof(int));
    int *displs = malloc(comm_size * sizeof(int));
    int base = n_items / comm_size;
    int rem = n_items % comm_size;
    for (int i = 0; i < comm_size; i++) {
        counts[i] = base + (i < rem ? 1 : 0);
        displs[i] = (i == 0) ? 0 : displs[i - 1] + counts[i - 1];
    }

    int local_n = counts[my_rank];
    float* local_array = malloc(local_n * sizeof(float));

    if (my_rank == 0) {
        global_array = read_input(inputFile, n_items);
        fclose(inputFile);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double local_start = MPI_Wtime();

    // Scatter data
    MPI_Scatterv(global_array, counts, displs, MPI_FLOAT, local_array, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Local sort
    qsort(local_array, local_n, sizeof(float), cmpfloat);

    // Parallel tree merge
    int step = 1;
    while (step < comm_size) {
        if (my_rank % (2 * step) == 0) {
            if (my_rank + step < comm_size) {
                int recv_n;
                MPI_Recv(&recv_n, 1, MPI_INT, my_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                float* recv_array = malloc(recv_n * sizeof(float));
                MPI_Recv(recv_array, recv_n, MPI_FLOAT, my_rank + step, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                float* merged = merge_arrays(local_array, local_n, recv_array, recv_n);
                free(local_array);
                free(recv_array);
                local_array = merged;
                local_n += recv_n;
            }
        } else {
            int dest = my_rank - step;
            MPI_Send(&local_n, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            MPI_Send(local_array, local_n, MPI_FLOAT, dest, 1, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    double local_finish = MPI_Wtime();
    double local_elapsed = local_finish - local_start;
    double elapsed;

    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        FILE* outputFile = fopen(argv[3], "w");
        FILE* timeFile   = fopen(argv[4], "w");

        for (int i = 0; i < local_n; i++) {
            fprintf(outputFile, "%.6f%s", local_array[i], (i == local_n - 1 ? "" : ","));
        }
        fprintf(timeFile, "%.6f\n", elapsed);

        fclose(outputFile);
        fclose(timeFile);
        free(global_array);
    }

    free(counts);
    free(displs);
    free(local_array);

    MPI_Finalize();
    if (DEBUG) printf("Finished!\n");
    return 0;
}


/* Read Input -------------------- */
float* read_input(FILE* inputFile, int n_items) {
    float* arr = (float*)malloc(n_items * sizeof(float));
    char line[MAXLINE] = {0};
    int i = 0;
    while (fgets(line, MAXLINE, inputFile)) {
        sscanf(line, "%f", &(arr[i++]));
    }
    return arr;
}


/* Cmp Int ----------------------------- */
// use this for qsort
// source: https://stackoverflow.com/questions/3886446/problem-trying-to-use-the-c-qsort-function
int cmpfloat(const void* a, const void* b) {
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return COMPARE(fa, fb);
}


/* Merge Arrays ------------------ */
float* merge_arrays(float* a, int a_len, float* b, int b_len) {
    float* result = malloc((a_len + b_len) * sizeof(float));
    int i = 0, j = 0, k = 0;
    while (i < a_len && j < b_len) {
        result[k++] = (a[i] < b[j]) ? a[i++] : b[j++];
    }
    while (i < a_len) result[k++] = a[i++];
    while (j < b_len) result[k++] = b[j++];
    return result;
}
