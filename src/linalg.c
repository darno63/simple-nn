#include <math.h>
#include <stdlib.h>

#include "linalg.h"

/*
********************************************** 
*           Matrix Math Functions            *
********************************************** 
*/

/* matrix init function */
Matrix* make_matrix(int m, int n, int random) {
  double **values = malloc(m * sizeof(double*));
  for (int i = 0; i < m; i++) {
    values[i] = malloc(n * sizeof(double));
    for (int j=0;j<n;j++) {
      if (random == 0) {
        values[i][j] = 0;
      } else {
        //float in range -1 to 1
        values[i][j] = (double)rand()/RAND_MAX*2.0-1.0;
      }
    }
  }
  Matrix* res = malloc(sizeof(Matrix));
  res->m = m;
  res->n = n;
  res->values = values;
  return res;
}

/* matrix assignment function */
Matrix* assign_matrix(int m, int n, double *array) {
  double **values = malloc(m * sizeof(double*));
  for (int i = 0; i < m; i++) {
    values[i] = malloc(n * sizeof(double));
    for (int j=0;j<n;j++) {
      values[i][j] = array[i * n + j];
    }
  }
  Matrix* res = malloc(sizeof(Matrix));
  res->m = m;
  res->n = n;
  res->values = values;
  return res;
}

/* free matrix */
void free_matrix(Matrix* mat) {
  for (int i=0;i<(mat->m);i++) {
    free(mat->values[i]);
  }
  free(mat->values);
  free(mat);
}

 /* matrix transpose */
Matrix* transpose(Matrix mat) {
  Matrix* new_mat = make_matrix(mat.n, mat.m, 0);
  for (int i=0;i<new_mat->m;i++) {
    for (int j=0;j<new_mat->n;j++) {
      new_mat->values[i][j] = mat.values[j][i];
    }
  }
  return new_mat;
}
 /* new matrix transpose */
Matrix* new_transpose(Matrix* mat, Matrix* m2) {
  for (int i=0;i<mat->m;i++) {
    for (int j=0;j<mat->n;j++) {
      m2->values[i][j] = mat->values[j][i];
    }
  }
  return m2;
}
/* matrix multiplication */
Matrix* matmul(Matrix* m1, Matrix* m2){
  int m = m1->m;
  int n = m2->n;
  //Matrix* res_mat = malloc(sizeof(Matrix));
  Matrix* res_mat = make_matrix(m, n, 0);
  //Matrix res_mat = make_matrix(m, n, 0);
  for (int i=0;i<m;i++) {
    for (int j=0;j<m2->n;j++) {
      for (int k=0;k<m2->m;k++) {
        res_mat->values[i][j] += m1->values[i][k] * m2->values[k][j];
      }
    }
  }
  return res_mat;
}

