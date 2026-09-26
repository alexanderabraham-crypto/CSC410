#include "matrixT.h"
#include <time.h>  // Added for timing

int main() 
{
    // declare thread id and thread data
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    
    // Dynamically allocate memory for the matrices
    A = (int**)malloc(N * sizeof(int*));
    B = (int**)malloc(N * sizeof(int*));
    C = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; ++i) {
        A[i] = (int*)malloc(N * sizeof(int));
        B[i] = (int*)malloc(N * sizeof(int));
        C[i] = (int*)malloc(N * sizeof(int));
    }

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    // Initialize matrices A and B with values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    printf("Matrices initialized successfully.\n");

    clock_t start = clock();  // Added start time

    // Create threads to perform matrix multiplication
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].num_rows = N / NUM_THREADS;

        pthread_create(
            &threads[i],
            NULL,
            matrixMultiplyThread,
            &thread_data[i]
        );
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();  // Added end time

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;  // Added execution time

    printf("Matrix multiplication complete!\n");
    printf("Execution time: %.6f seconds\n", elapsed);  // Added execution time

    // Optionally, display the resulting matrix C (Not when you are timing :) )
    // displayMatrix(C, N); So this way it wasn't an endless parade of zeroes.

    // Free dynamically allocated memory
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
