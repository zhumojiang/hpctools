#include <mkl_lapacke.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "timer.h"
#include "dgesv.h"

double *generate_matrix(unsigned int size, unsigned int seed)
{
  unsigned int i;
  double *matrix = (double *) aligned_alloc(32,sizeof(double) * size * size);

  srand(seed);

  for (i = 0; i < size * size; i++) {
    matrix[i] = rand() % 100;
  }

  return matrix;
}

double *duplicate_matrix(double *orig, unsigned int size)
{
  double *replica = (double *) aligned_alloc(32,sizeof(double) * size * size);

  memcpy((void *) replica, (void *) orig, size * size * sizeof(double));

  return replica;
}

int is_nearly_equal(double x, double y)
{
  const double epsilon = 1e-5 ;
  return fabs(x - y) <= epsilon * fabs(x);
 
}

unsigned int check_result(double *bref, double *b, unsigned int size, unsigned int nrhs)
{
  unsigned int i;

  for(i = 0; i < size * nrhs; i++) {
    if (!is_nearly_equal(bref[i], b[i]))
      return 0;
  }

  return 1;
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("You need to provide a matrix size (e.g. 1024 for use 1024x1024 matrices)\n");
    return 1;
  }

  int size = atoi(argv[1]);

  double *a, *aref;
  double *b, *bref;

  a = generate_matrix(size, 1);
  b = generate_matrix(size, 2);
  aref = duplicate_matrix(a, size);
  bref = duplicate_matrix(b, size);

  int n = size, nrhs = size, lda = size, ldb = size, info;
  int *ipiv = (int *) aligned_alloc(32,sizeof(int) * size);

  timeinfo start, now;
  timestamp(&start);

  info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, n, nrhs, aref, lda, ipiv, bref, ldb);

  timestamp(&now);
  printf("Time taken by Lapacke dgesv: %ld ms\n", diff_milli(&start, &now));

  timestamp(&start);

  info = my_dgesv(n, nrhs, a, b);

  timestamp(&now);
  printf("Time taken by my dgesv solver: %ld ms\n", diff_milli(&start, &now));

  if (check_result(bref, b, size, nrhs) == 1)
    printf("Result is ok!\n");
  else
    printf("Result is wrong!\n");


  free(a);
  free(b);
  free(aref);
  free(bref);
  free(ipiv);

  return 0;
}
