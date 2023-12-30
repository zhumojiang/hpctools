#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ��������
double* copy_matrix(int n, double* a);

int my_dgesv(int n, int nrhs, double* a, double* b) {
    int i, j, k;

    // ����a�ĸ����������޸�ԭʼ����
    double* a_copy = copy_matrix(n, a);
    if (!a_copy) {
        return -1; // �ڴ����ʧ��
    }

    // ���и�˹��Ԫ
    for (k = 0; k < n - 1; k++) {
        if (fabs(a_copy[k * n + k]) < 1e-10) {
            free(a_copy); // �ͷ��ڴ�
            return -1; // �������
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

    // �ش����x
    for (i = n - 1; i >= 0; i--) {
        for (j = 0; j < nrhs; j++) {
            double sum = b[i * nrhs + j];
            for (k = i + 1; k < n; k++) {
                sum -= a_copy[i * n + k] * b[k * nrhs + j];
            }
            if (fabs(a_copy[i * n + i]) < 1e-10) {
                free(a_copy); // �ͷ��ڴ�
                return -1; // �������
            }
            b[i * nrhs + j] = sum / a_copy[i * n + i];
        }
    }

    free(a_copy); // �ͷŸ����ڴ�
    return 0;
}

// ���������ؾ���ĸ���
double* copy_matrix(int n, double* a) {
    double* copy = malloc(n * n * sizeof(double));
    if (copy) {
        for (int i = 0; i < n * n; i++) {
            copy[i] = a[i];
        }
    }
    return copy;
}
