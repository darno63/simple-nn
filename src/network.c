#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "network.h"

/*
********************************************** 
*  Layer/Network Structs & Create Functions  *
********************************************** 

*/
/* layer init function 
 *
 */
Layer* create_layer(int m, int n, int random, Matrix* (*activation)(Matrix*)) {
  Layer* res = malloc(sizeof(Layer));
  res->m = m;
  res->n = n;
  res->act_func = *activation;
  res->weights = make_matrix(m,n,1);
  res->wderivs = make_matrix(m,n,0);
  res->outputs = make_matrix(n,1,0);
  return res;
} 

/* Create Network */
Network* create_network(double *inputs, int ninputs, Layer **layers, int nlayers) {
  Network* res = malloc(sizeof(Network));
  res->inputs = assign_matrix(ninputs, 1, inputs);
  res->layers = layers;
  res->nlayers = nlayers;
  return res;
}

/* free Network memory */
void free_network(Network* network) {
  free_matrix(network->inputs);
  for (int i=0;i<(network->nlayers);i++) {
    free_matrix(network->layers[i]->weights);
    free_matrix(network->layers[i]->wderivs);
    free_matrix(network->layers[i]->outputs);
    free(network->layers[i]);
  }
  free(network);
}

/* relu */
Matrix* relu(Matrix* mat) {
  Matrix* res_mat = make_matrix(mat->m, mat->n, 0);
  for (int i=0;i<mat->m;i++) {
    for (int j=0;j<mat->n;j++) {
      if (mat->values[i][j] < 0){
        res_mat->values[i][j] = 0;
      } else {
        res_mat->values[i][j] = mat->values[i][j];
      }
    }
  }
  return res_mat;
}

/* sigmoid */
Matrix* sigmoid(Matrix* mat) {
  Matrix* res_mat = make_matrix(mat->m, mat->n, 0);
  for (int i=0;i<mat->m;i++) {
    for (int j=0;j<mat->n;j++) {
      res_mat->values[i][j] = 1 / (1 + exp(-mat->values[i][j]));
    }
  }
  return res_mat;
}

/* sigmoid deriv*/
Matrix* sigmoid_deriv(Matrix* mat) {
  Matrix* res_mat = make_matrix(mat->m, mat->n, 0);
  for (int i=0;i<mat->m;i++) {
    for (int j=0;j<mat->n;j++) {
      res_mat->values[i][j] = mat->values[i][j] * (1 - mat->values[i][j]);
    }
  }
  return res_mat;
}

// change arg to Network pointer
/** Foward Propagation */
Matrix* forward_prop(Network* network) {
  Matrix* net = matmul(network->layers[0]->weights, network->inputs);
  free_matrix(network->layers[0]->outputs);
  network->layers[0]->outputs = network->layers[0]->act_func(net);
  free_matrix(net);
  if (network->nlayers > 1) {
    for (int i=1;i<network->nlayers;i++) {
      Matrix* lnet = matmul(network->layers[i]->weights, network->layers[i-1]->outputs);
      free_matrix(network->layers[i]->outputs);
      network->layers[i]->outputs = network->layers[i]->act_func(lnet);
      free_matrix(lnet);
    }
  }
  return network->layers[network->nlayers-1]->outputs;
}

/** Back Propagation */
// https://towardsdatascience.com/derivative-of-the-sigmoid-function-536880cf918e

// find weights gradient
Matrix* dfdw(Matrix* w, Matrix* outputs_d, Matrix* inputs) {
  double *arr = malloc(inputs->m * outputs_d->m * sizeof(double));
  int input_i = 0;
  int output_i = 0;
  for (int i=0; i< inputs->m * outputs_d->m; i++) {
    arr[i] = inputs->values[input_i][0] * outputs_d->values[output_i][0];
    input_i++;
    if (input_i == inputs->m) {
      output_i++;
      input_i = 0;
    }
  }
  Matrix* res = assign_matrix(w->m, w->n, arr);
  free(arr);
  return res;
}

// back propagation function
void back_prop(Network* network) {
  for (int i=network->nlayers-1;i>=0;i--) {
    Matrix* actderiv = sigmoid_deriv(network->layers[i]->outputs);
    // find df/dw
    if (i > 0) {
      Matrix* w_d = dfdw(network->layers[i]->weights, actderiv, network->layers[i-1]->outputs);
      free_matrix(network->layers[i]->wderivs);
      network->layers[i]->wderivs = w_d;
    } else {
      Matrix* w_d = dfdw(network->layers[i]->weights, actderiv, network->inputs);
      free_matrix(network->layers[i]->wderivs);
      network->layers[i]->wderivs = w_d;
    }
    free_matrix(actderiv);
  }
}

/* matrix printer */
void print_mat(Matrix* mat) {
  int m = mat->m;
  int n = mat->n;
  for (int i=0;i<m;i++) {
    for (int j=0;j<n;j++) {
      if (j == n-1) {
        printf("%f\n", mat->values[i][j]);
      } else {
        printf("%f, ", mat->values[i][j]);
      }
    }
  }
}

