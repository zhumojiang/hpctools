Task: Use Valgrind Memcheck to check your serial implementation and identify possible memory issues. What problems did you discover? What are they specifically? Resolve these issues and obtain a clean Valgrind run.

During the first run of "make memcheck," multiple memory leaks were detected in the code in main.c. The specific problems are as follows:

1. In the "generate_matrix" function:
   - This function uses "malloc" to allocate memory for creating a matrix, but there is no corresponding "free" call in the main function to release this memory.

2. In the "duplicate_matrix" function:
   - Similarly, this function allocates memory for copying a matrix, but the code lacks the necessary memory release in the main function.

To resolve these memory leaks, the allocated memory for these matrices needs to be freed when they are no longer needed. Add "free" function calls at the end of the main function to release the memory.

During the second run of "make memcheck," the following two issues were detected:

1. Execution time:
   - "Time taken by Lapacke dgesv: 148806 ms": Using the LAPACKE library's "dgesv" function to solve a linear equation system took approximately 148.8 seconds.
   - "Time taken by my dgesv solver: 75402 ms": The "dgesv" solver solving the same linear equation system took approximately 75.4 seconds.

2. Result correctness:
   - "Result is wrong!"

No memory leaks were detected in the program. However, the solver's calculation result is inconsistent with the result from the LAPACKE library, indicating a need to check and correct the algorithm.

The modified dgesv algorithm is as follows:

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function declaration
double* copy_matrix(int n, double* a);

int my_dgesv(int n, int nrhs, double* a, double* b) {
    int i, j, k;

    // Create a copy of a to avoid modifying the original matrix
    double* a_copy = copy_matrix(n, a);
    if (!a_copy) {
        return -1; // Memory allocation failed
    }

    // Perform Gaussian elimination
    for (k = 0; k < n - 1; k++) {
        if (fabs(a_copy[k * n + k]) < 1e-10) {
            free(a_copy); // Release memory
            return -1; // Singular matrix
        }
        for (i = k + 1; i < n; i++) {
            double factor = -a_copy[i * n + k] / a_copy[k * n + k];
            for (j = k; j < n; j++) {
                a_copy[i * n + j] += factor * a_copy[k * n + j];
            }
            for (j = 0; j < nrhs; j++) {
                b[i * nrhs + j] += factor * b[k * nrhs + j];
            }
        }
    }

    // Back-substitution to solve for x
    for (i = n - 1; i >= 0; i--) {
        for (j = 0; j < nrhs; j++) {
            double sum = b[i * nrhs + j];
            for (k = i + 1; k < n; k++) {
                sum -= a_copy[i * n + k] * b[k * nrhs + j];
            }
            if (fabs(a_copy[i * n + i]) < 1e-10) {
                free(a_copy); // Release memory
                return -1; // Singular matrix
            }
            b[i * nrhs + j] = sum / a_copy[i * n + i];
        }
    }

    free(a_copy); // Release copy memory
    return 0;
}

// Create and return a copy of the matrix
double* copy_matrix(int n, double* a) {
    double* copy = malloc(n * n * sizeof(double));
    if (copy) {
        for (int i = 0; i < n * n; i++) {
            copy[i] = a[i];
        }
    }
    return copy;
}

During the third run of "make memcheck," a clean result is obtained.