// convert sequential sums to parallel

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_PROCESSES 4

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <N> <number_of_processes>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int num_processes = atoi(argv[2]);

    if (N <= 0) {
        fprintf(stderr, "N must be greater than 0.\n");
        return 1;
    }

    if (num_processes < 1 || num_processes > MAX_PROCESSES) {
        fprintf(stderr, "Number of processes must be between 1 and 4.\n");
        return 1;
    }

    int *arr = malloc(N * sizeof(int));

    if (!arr) {
        perror("malloc");
        return 1;
    }

    // Fill the array
    for (int i = 0; i < N; i++) {
        arr[i] = i + 1;
    }

    // Create one pipe for each possible child
    int pipes[MAX_PROCESSES][2];

    // Store child process IDs
    pid_t pids[MAX_PROCESSES];

    // Calculate approximately equal chunk sizes
    int chunk_size = N / num_processes;
    int remainder = N % num_processes;

    int start = 0;

    // Create child processes
    for (int i = 0; i < num_processes; i++) {

        // Create pipe
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            free(arr);
            return 1;
        }

        // Determine this child's chunk size
        int chunk = chunk_size;

        // Give the first few children one extra element
        if (i < remainder) {
            chunk++;
        }

        int end = start + chunk;

        // Create child
        pids[i] = fork();

        // Check if process creation failed
        if (pids[i] < 0) {
            perror("fork");
            free(arr);
            return 1;
        }

        // Child process
        if (pids[i] == 0) {

            // Child only needs the write end
            close(pipes[i][0]);

            long long partial_sum = 0;

            // Calculate sum of this child's chunk
            for (int j = start; j < end; j++) {
                partial_sum += arr[j];
            }

            // Send partial sum to parent through pipe
            if (write(pipes[i][1],
                      &partial_sum,
                      sizeof(partial_sum)) == -1) {
                perror("write");
                close(pipes[i][1]);
                free(arr);
                exit(1);
            }

            // Close write end
            close(pipes[i][1]);

            free(arr);

            // Child is finished
            exit(0);
        }

        // Parent moves to the next chunk
        start = end;
    }

    // Parent does not write to the pipes
    for (int i = 0; i < num_processes; i++) {
        close(pipes[i][1]);
    }

    // Wait for all children to finish
    for (int i = 0; i < num_processes; i++) {
        waitpid(pids[i], NULL, 0);
    }

    // Collect partial sums from each child
    long long total = 0;

    for (int i = 0; i < num_processes; i++) {

        long long partial_sum;

        if (read(pipes[i][0],
                 &partial_sum,
                 sizeof(partial_sum)) == -1) {
            perror("read");
            close(pipes[i][0]);
            free(arr);
            return 1;
        }

        total += partial_sum;

        // Close read end
        close(pipes[i][0]);
    }

    printf("Total sum = %lld\n", total);

    free(arr);

    return 0;
}
