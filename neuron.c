#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// https://levelup.gitconnected.com/advanced-concepts-in-c-structures-254fe156b3fb


/*
********************************************** 
*           Matrix Math Functions            *
********************************************** 
*/

/* create matrix struct */
typedef struct Matrix {
int m;
int n;
double **values;
} Matrix;

/* matrix init function */
Matrix make_matrix(int m, int n, int random) {
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
  Matrix res = { m, n, values};
  return res;
}

/* matrix assignment function */
Matrix assign_matrix(int m, int n, double *array) {
  double **values = malloc(m * sizeof(double*));
  for (int i = 0; i < m; i++) {
    values[i] = malloc(n * sizeof(double));
    for (int j=0;j<n;j++) {
      values[i][j] = array[i * n + j];
    }
  }
  Matrix res = { m, n, values};
  return res;
}

/* free matrix */
void free_matrix(Matrix* mat) {
  for (int i=0;i<(mat->m);i++) {
    free(mat->values[i]);
  }
  free(mat->values);
}

 /* matrix transpose */
Matrix transpose(Matrix mat) {
  Matrix new_mat = make_matrix(mat.n, mat.m, 0);
  for (int i=0;i<new_mat.m;i++) {
    for (int j=0;j<new_mat.n;j++) {
      new_mat.values[i][j] = mat.values[j][i];
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
Matrix matmul(Matrix m1, Matrix m2){
  int m = m1.m;
  int n = m2.n;
  Matrix res_mat = make_matrix(m, n, 0);
  for (int i=0;i<m;i++) {
    for (int j=0;j<m2.n;j++) {
      for (int k=0;k<m2.m;k++) {
        res_mat.values[i][j] += m1.values[i][k] * m2.values[k][j];
      }
    }
  }
  return res_mat;
}

/*
********************************************** 
*  Layer/Network Structs & Create Functions  *
********************************************** 
*/

/* create Layer struct */
typedef struct Layer Layer;
struct Layer {
  int m;
  int n;
  Matrix (*act_func)(Matrix);
  Matrix weights;
  Matrix wderivs;
  Matrix outputs;
  //Layer *next_layer;
  //Layer *last_layer;
};

/* layer init function 
 *
 */
Layer* create_layer(int m, int n, int random, Matrix (*activation)(Matrix)) {
  Layer* res = malloc(sizeof(Layer));
  res->m = m;
  res->n = n;
  res->act_func = *activation;
  res->weights = make_matrix(m,n,1);
  res->wderivs = make_matrix(m,n,0);
  res->outputs = make_matrix(n,1,0);
    //NULL,
    //NULL};
  return res;
} 

/* Network */
typedef struct Network {
  Matrix inputs;
  Layer *layers;
  int nlayers;
} Network;

/* Create Network */
Network create_network(double *inputs, int ninputs, Layer *layers, int nlayers) {
  // transpose the inputs
  Matrix new_inputs = assign_matrix(ninputs, 1, inputs);

  /* connect layers
  for (int i=0;i<nlayers;i++) {
    if (i != 0) {
      layers[i].last_layer = &layers[i-1];
    }
    if (i != nlayers -1) {
      layers[i].next_layer = &layers[i+1];
    }
  }  
  */
  Network res = {new_inputs, layers, nlayers};
  return res;
}

/* free Network memory */
void free_network(Network* network) {
  free_matrix(&network->inputs);
  for (int i=0;i<(network->nlayers);i++) {
    free_matrix(&network->layers[i].weights);
    free_matrix(&network->layers[i].wderivs);
    free_matrix(&network->layers[i].outputs);
  }
}

/* relu */
Matrix relu(Matrix mat) {
  Matrix res_mat = make_matrix(mat.m, mat.n, 0);
  for (int i=0;i<mat.m;i++) {
    for (int j=0;j<mat.n;j++) {
      if (mat.values[i][j] < 0){
        res_mat.values[i][j] = 0;
      } else {
        res_mat.values[i][j] = mat.values[i][j];
      }
    }
  }
  return res_mat;
}

/* sigmoid */
Matrix sigmoid(Matrix mat) {
  Matrix res_mat = make_matrix(mat.m, mat.n, 0);
  for (int i=0;i<mat.m;i++) {
    for (int j=0;j<mat.n;j++) {
      res_mat.values[i][j] = 1 / (1 + exp(-mat.values[i][j]));
    }
  }
  return res_mat;
}

/* sigmoid deriv*/
Matrix sigmoid_deriv(Matrix mat) {
  Matrix res_mat = make_matrix(mat.m, mat.n, 0);
  for (int i=0;i<mat.m;i++) {
    for (int j=0;j<mat.n;j++) {
      res_mat.values[i][j] = mat.values[i][j] * (1 - mat.values[i][j]);
    }
  }
  return res_mat;
}

/** Foward Propagation */
Matrix forward_prop(Network network) {
  // change back to relu
  Matrix net = matmul(network.layers[0].weights, network.inputs);
  network.layers[0].outputs = network.layers[0].act_func(net);
  free_matrix(&net);
  if (network.nlayers > 1) {
    for (int i=1;i<network.nlayers;i++) {
      Matrix lnet = matmul(network.layers[i].weights, network.layers[i-1].outputs);
      network.layers[i].outputs = network.layers[i].act_func(lnet);
      free_matrix(&lnet);
    }
  }
  return network.layers[network.nlayers-1].outputs;
}

/** Back Propagation */
// https://towardsdatascience.com/derivative-of-the-sigmoid-function-536880cf918e

// find weights gradient
Matrix dfdw(Matrix w, Matrix outputs_d, Matrix inputs) {
  double *arr = malloc(inputs.m * outputs_d.m * sizeof(double));
  int input_i = 0;
  int output_i = 0;
  for (int i=0; i< inputs.m * outputs_d.m; i++) {
    arr[i] = inputs.values[input_i][0] * outputs_d.values[output_i][0];
    input_i++;
    if (input_i == inputs.m) {
      output_i++;
      input_i = 0;
    }
  }
  Matrix res = assign_matrix(w.m, w.n, arr);
  return res;
}

// back propagation function
void back_prop(Layer *layer, Matrix inputs) {
  //Matrix actderiv = sigmoid_deriv(network.layers[network.nlayers -1].outputs)
  Matrix actderiv = sigmoid_deriv(layer->outputs);
  // find df/dw
  Matrix w_d = dfdw(layer->weights, actderiv, inputs);
  layer->wderivs = w_d;
}

/* matrix printer */
void print_mat(Matrix mat) {
  int m = mat.m;
  int n = mat.n;
  for (int i=0;i<m;i++) {
    for (int j=0;j<n;j++) {
      printf("%f ", mat.values[i][j]);
    }
    printf("\n");
  }
}

/* example of a layer function */
void ex_func() {
}

/* matrix testing */
void ex1() {
  Layer* layer1 = create_layer(3, 3, 1, relu);
  Layer* layer2 = create_layer(2, 3, 1, relu);
  Layer* layer3 = create_layer(3, 2, 1, sigmoid);
  Layer layers[3] = {*layer1, *layer2, *layer3};
  double inputs[3] = {1, 2, 3};
  Network nn = create_network(inputs, 3, layers, 3);
  printf("network inputs\n");
  print_mat(nn.inputs);
  printf("\n");
  Matrix output = forward_prop(nn);
  for (int i=0;i<3;i++) {
    printf("layer%d weights\n", i);
    print_mat(nn.layers[i].weights);
    printf("layer%d outputs\n", i);
    print_mat(nn.layers[i].outputs);
    printf("\n");
  }
  free_network(&nn);
  free(layer1);
  free(layer2);
  free(layer3);
}

/* backprop testing */
void ex2() {
  Layer* layer1 = create_layer(4, 5, 1, relu);
  Layer layers[1] = {*layer1};
  Matrix ignore = make_matrix(1,5,1);
  double inputs[5] = {0.680375, -0.211234, 0.566198, 0.596880, 0.823295};
  Network nn = create_network(inputs, 5, layers, 1);
  printf("network inputs\n");
  print_mat(nn.inputs);
  printf("\n");
  Matrix output = forward_prop(nn);
  for (int i=0;i<1;i++) {
    printf("layer%d weights\n", i);
    print_mat(nn.layers[i].weights);
    printf("layer%d outputs\n", i);
    print_mat(nn.layers[i].outputs);
    printf("\n");
  }
  // backprop
  back_prop(&nn.layers[0], nn.inputs);
  print_mat(nn.layers[0].wderivs); 
}

void ex3() {
  Matrix inputs = make_matrix(5,1,1);
  printf("inputs\n");
  print_mat(inputs);
  printf("\n");
  Matrix weights = make_matrix(4,5,1);
  printf("weights\n");
  print_mat(weights);
  printf("\n");
  Matrix net = matmul(weights, inputs);
  printf("net\n");
  print_mat(net);
  printf("\n");
  Matrix act = sigmoid(net);
  printf("act\n");
  print_mat(act);
  printf("\n");
  Matrix act_d = sigmoid_deriv(act);
  printf("act_d\n");
  print_mat(act_d);
  printf("\n");
  Matrix res = dfdw(weights, act_d, inputs);
  print_mat(res);
}

int main() {
  ex1();
  return 0;
}
