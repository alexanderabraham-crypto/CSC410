#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000  // Size of the matrix
#define NUM_THREADS 4  // Number of threads
                       // Changed from 4 to 1
                       // Changed from 1 to 2 
                       // Changed from 2 to 3
                       // Back to 4

int **A, **B, **C;  // Global matrices

// Structure to hold information for each thread
typedef struct 
{
    int thread_id;
    int num_rows;  // Number of rows each thread will handle
} thread_data_t;

// Function for each thread to perform matrix multiplication
void* matrixMultiplyThread(void* arg) 
{
    // Divide the task (rows) of each thread based on thread id
    thread_data_t* data = (thread_data_t*)arg;

    int start = data->thread_id * data->num_rows;
    int end = start + data->num_rows;

    // compute a portion of the matrix multiplication
    for (int i = start; i < end; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;

            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Return from thread
    return NULL;
}

void displayMatrix(int** matrix, int n) 
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
