Task：Parallelize your implementation of the routine dgesv using OpenMP. It is not required that you make a perfect parallelization, just, something parallel to work with. How did you parallelize your code? How much faster (speedup) is the parallelized code than the serial one?
Use Intel Vtune Amplifier to identify the hotspots of your parallelized code. Which are they?


Modify dgesv.c to implement parallel computing through OpenMP.

    // 进行高斯消元
    for (k = 0; k < n - 1; k++) {
        if (fabs(a_copy[k * n + k]) < 1e-10) {
            free(a_copy); // 释放内存
            return -1; // 奇异矩阵
        }
#pragma omp parallel for private(i, j, factor)
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


Due to the inherent sequential nature of the Gaussian elimination algorithm, it is impossible to completely eliminate data dependencies. This data dependency is an intrinsic characteristic of the algorithm. Similarly, automatic vectorization cannot be achieved, as it would change the algorithm itself.