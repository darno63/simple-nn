#include "linalg.h"

/* create Layer struct */
typedef struct Layer Layer;
struct Layer {
  int m;
  int n;
  Matrix* (*act_func)(Matrix*);
  Matrix* weights;
  Matrix* wderivs;
  Matrix* outputs;
};

/* Network */
typedef struct Network {
  Matrix* inputs;
  Layer **layers;
  int nlayers;
} Network;

Layer* create_layer(int m, int n, int random, Matrix* (*activation)(Matrix*));

Network* create_network(double *inputs, int ninputs, Layer **layers, int nlayers);
void free_network(Network* network);
Matrix* relu(Matrix* mat);
Matrix* sigmoid(Matrix* mat);
Matrix* sigmoid_deriv(Matrix* mat);
Matrix* forward_prop(Network* network);
Matrix* dfdw(Matrix* w, Matrix* outputs_d, Matrix* inputs);
void back_prop(Network* network);
void print_mat(Matrix* mat);
