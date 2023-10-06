#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int my_dgesv(int n, int nrhs, double *a, double *b) {
    int i, j, k;

    for (k = 0; k < n - 1; k++) {
        if (fabs(a[k * n + k]) < 1e-10) {
            // Handle the case where the pivot element is zero, which may indicate a singular matrix
            return -1; // Return an error code indicating a singular matrix
        }
        for (i = k + 1; i < n; i++) {
            double t = -a[i * n + k] / a[k * n + k];
            for (j = k; j < n; j++)
                a[i * n + j] += t * a[k * n + j];
            for (j = 0; j < nrhs; j++)
                b[i * nrhs + j] += t * b[k * nrhs + j];
        }
    }

    for (i = n - 1; i >= 0; i--) {
        double t = b[i * nrhs];
        for (j = i + 1; j < n; j++)
            t -= a[i * n + j] * b[j * nrhs];
        if (fabs(a[i * n + i]) < 1e-10) {
            // Handle the case where the pivot element is zero, which may indicate a singular matrix
            return -1; // Return an error code indicating a singular matrix
        }
        b[i * nrhs] = t / a[i * n + i];
    }

    return 0; 
}
