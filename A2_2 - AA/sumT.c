#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>  // Added for timing

#define SIZE 100000000
#define NUM_THREADS 4  // Changed from 4 to 1 for this test
                       // Changed from 1 to 2
                       // Changed from 2 to 3
                       // Changed from 3 to 4

long long arr[SIZE];
long long partialSums[NUM_THREADS] = {0};  // Changed int to long long

// Entry function for each thread
void* sumPart(void* arg) 
{
    // Divide the work for each thread based on their id and let them compute partial sums
    int thread_id = *(int*)arg;  // Added to get the thread ID

    int start = thread_id * (SIZE / NUM_THREADS);  // Added starting position
    int end = start + (SIZE / NUM_THREADS);  // Added ending position

    for (int i = start; i < end; i++) {  // Added loop for this threads section
        partialSums[thread_id] += arr[i];  // Added partial sum
    }

    return NULL;  // Added return for thread
}

int main() 
{
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    clock_t start = clock();  // Added start time

    // Create threads to compute partial sums
    for (int i = 0; i < NUM_THREADS; i++) {  // Added thread creation loop
        thread_ids[i] = i;  // Added thread ID

        pthread_create(
            &threads[i],
            NULL,
            sumPart,
            &thread_ids[i]
        );  // Added thread creation
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {  // Added thread waiting loop
        pthread_join(threads[i], NULL);  // Added thread join
    }

    clock_t end = clock();  // Added end time

    // Combine the partial sums from all threads
    long long totalSum = 0;  // Changed int to long long
    for (int i = 0; i < NUM_THREADS; i++) {
        totalSum += partialSums[i];
    }

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;  // Added execution time

    // Print the total sum;
    printf("Total Sum: %lld\n", totalSum);  // Changed %d to %lld
    printf("Execution time: %.6f seconds\n", elapsed);  // Added execution time

    return 0;
}
