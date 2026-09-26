#include <stdio.h>
#include <time.h>

#define SIZE 100000000  // Changed from 10000 to 100000000

long long arr[SIZE];  // Changed to global array to avoid stack overflow

long long sumArray(long long arr[], int size)  // Changed int to long long
{
    long long sum = 0;  // Changed int to long long

    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }

    return sum;
}

int main() 
{
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    clock_t start = clock();

    long long totalSum = sumArray(arr, SIZE);  // Changed int to long long

    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Total Sum: %lld\n", totalSum);  // Changed %d to %lld
    printf("Execution time: %.6f seconds\n", elapsed);

    return 0;
}
