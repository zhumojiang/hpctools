//#include <lapacke.h>
#include <openblas/lapacke.h>
//#include <mkl_lapacke.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dgesv.h"
#include <string.h>

int main() {
    // The coefficient matrix A of a linear system.
    double A[9] = {
        2.0, 3.0, -1.0,
        5.0, 2.0, 2.0,
        1.0, -1.0, 3.0
    };

    // Solution matrix B (now 3x3)
    double B[9] = {
        4.0, 1.0, -2.0,
        3.0, 5.0, 7.0,
        8.0, 2.0, 6.0
    };

    // Create copies for comparison
    double A_copy[9];
    double B_copy[9];
    memcpy(A_copy, A, sizeof(A));
    memcpy(B_copy, B, sizeof(B));

    // Solve the linear system using LAPACK dgesv
    int n = 3, nrhs = 3, lda = 3, ldb = 3, info;
    int *ipiv = (int *) malloc(sizeof(int) * n);
    info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, n, nrhs, A_copy, lda, ipiv, B_copy, ldb);

    // Solve the linear system using my solver
    my_dgesv(n, nrhs, A, B);

    // Verify the results
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < nrhs; j++) {
            printf("x[%d][%d] = %f (Expected: %f)\n", i, j, B[i * ldb + j], B_copy[i * ldb + j]);
        }
    }

    free(ipiv);
    return 0;
}
