#include <stdio.h>
#include "network.h"

/* matrix testing */
void ex1() {

  printf("%lu\n", sizeof(Layer));
  Layer* layers[3] = {
    create_layer(3, 3, 1, relu),
    create_layer(2, 3, 1, relu),
    create_layer(3, 2, 1, sigmoid)
  };

  double inputs[3] = {1, 2, 3};
  Network* nn = create_network(inputs, 3, layers, 3);
  printf("network inputs\n");
  print_mat(nn->inputs);
  printf("\n");
  forward_prop(nn);
  for (int i=0;i<3;i++) {
    printf("layer%d weights\n", i);
    print_mat(nn->layers[i]->weights);
    printf("layer%d outputs\n", i);
    print_mat(nn->layers[i]->outputs);
    printf("\n");
  }
  free_network(nn);
}

/* backprop testing */
void ex2() {
  Layer* layer1 = create_layer(4, 5, 1, relu);
  Layer* layers[1] = {layer1};
  double inputs[5] = {0.680375, -0.211234, 0.566198, 0.596880, 0.823295};
  Network* nn = create_network(inputs, 5, layers, 1);
  printf("network inputs\n");
  print_mat(nn->inputs);
  printf("\n");
  forward_prop(nn);
  for (int i=0;i<1;i++) {
    printf("layer%d weights\n", i);
    print_mat(nn->layers[i]->weights);
    printf("layer%d outputs\n", i);
    print_mat(nn->layers[i]->outputs);
    printf("\n");
  }
  // backprop
  back_prop(nn);
  print_mat(nn->layers[0]->wderivs); 
}

/* backprop testing 2 */
void ex3() {
  Layer* layers[1] = {
   create_layer(4, 5, 1, relu)
  };
  double inputs[5] = {0.680375, -0.211234, 0.566198, 0.596880, 0.823295};
  Network* nn = create_network(inputs, 5, layers, 1);
  printf("network inputs\n");
  print_mat(nn->inputs);
  printf("\n");
  forward_prop(nn);
  for (int i=0;i<1;i++) {
    printf("layer%d weights\n", i);
    print_mat(nn->layers[i]->weights);
    printf("layer%d outputs\n", i);
    print_mat(nn->layers[i]->outputs);
    printf("\n");
  }
  // backprop
  back_prop(nn);
  print_mat(nn->layers[0]->wderivs); 
  free_network(nn);
}

void ex4() {
  /*
  Matrix inputs = make_matrix(5,1,1);
  printf("inputs\n"); print_mat(inputs);
  Matrix weights = make_matrix(4,5,1);
  printf("\nweights\n"); print_mat(weights);
  Matrix* net = matmul(&weights, &inputs);
  printf("\nnet\n"); print_mat(*net);
  Matrix act = sigmoid(*net);
  printf("\nact\n"); print_mat(act);
  Matrix act_d = sigmoid_deriv(act);
  printf("\nact_d\n"); print_mat(act_d);
  printf("\n");
  Matrix res = dfdw(weights, act_d, inputs);
  print_mat(res);
  */
}

int main() {
  ex1();
  return 0;
}
