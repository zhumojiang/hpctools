#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int my_dgesv(int n, int nrhs, double *a, double *b) {
    int i, j, k;

    
    for (k = 0; k < n - 1; k++) {
        if (fabs(a[k * n + k]) < 1e-10) {
            
            return -1; 
        }
        for (i = k + 1; i < n; i++) {
            double t = -a[i * n + k] / a[k * n + k];
            for (j = k; j < n; j++)
                a[i * n + j] += t * a[k * n + j];
            for (j = 0; j < nrhs; j++)
                b[i * nrhs + j] += t * b[k * nrhs + j];
        }
    }

    
    for (j = 0; j < nrhs; j++) { 
        for (i = n - 1; i >= 0; i--) {
            double t = b[i * nrhs + j];
            for (k = i + 1; k < n; k++)
                t -= a[i * n + k] * b[k * nrhs + j];
            if (fabs(a[i * n + i]) < 1e-10) {
               
                return -1; 
            }
            b[i * nrhs + j] = t / a[i * n + i];
        }
    }

    return 0; 
}
