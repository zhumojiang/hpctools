//#include <lapacke.h>
#include <openblas/lapacke.h>
//#include <mkl_lapacke.h>#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
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

    // Solution vector b
    double b[3] = {
        4.0,
        1.0,
        -2.0
    };

    // Create copies for comparison
    double A_copy[9];
    double b_copy[3];
    memcpy(A_copy, A, sizeof(A));
    memcpy(b_copy, b, sizeof(b));

    // Solve the linear system using LAPACK dgesv
    int n = 3, nrhs = 1, lda = 3, ldb = 1, info;
    int *ipiv = (int *) malloc(sizeof(int) * n);
    info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, n, nrhs, A_copy, lda, ipiv, b_copy, ldb);

    // Solve the linear system using my solver
    my_dgesv(n, nrhs, A, b);

    // Verify the results
    int i;
    for (i = 0; i < n; i++) {
        printf("x[%d] = %f (Expected: %f)\n", i, b[i], b_copy[i]);
    }

    return 0;
}