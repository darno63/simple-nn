/* create matrix struct */
typedef struct Matrix {
int m;
int n;
double **values;
} Matrix;

Matrix* make_matrix(int m, int n, int random);
Matrix* assign_matrix(int m, int n, double *array);
void free_matrix(Matrix* mat);
Matrix* transpose(Matrix mat);
Matrix* matmul(Matrix* m1, Matrix* m2);
