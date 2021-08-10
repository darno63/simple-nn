//#include <python3.8/cpython/initconfig.h>
#include <Python.h>
#include "network.h"

// practice func
void pprint(int *arr, double *arr2, unsigned char *arr3, int n) {
  for (int i=0;i<n;i++) {
    printf("%d\n", arr[i]);
    printf("%f\n", arr2[i]);
    printf("%u\n", arr3[i]);
  }
}

/* NN function */
void train(double *inputs, int ninputs, int* loutputs, int nlayers, int* linputs, int* activations) {
  Matrix* (*act_func[nlayers])(Matrix*);
  // get activation function
  for (int i=0;i<nlayers;i++) {
    switch(activations[i]) {
      case 0 :
        act_func[i] = relu;
        break;
      case 1 :
        act_func[i] = sigmoid;
        break;
      default :
        act_func[i] = relu;
    }
  }

  Layer* layers[nlayers];
  for (int i=0;i<nlayers;i++) {
    layers[i] = create_layer(loutputs[i], linputs[i], 1, act_func[i]);
  }
  Network* nn = create_network(inputs, ninputs, layers, nlayers);
  forward_prop(nn); 
  for (int i=0;i<nlayers;i++) {
    printf("layer%d weights\n", i);
    print_mat(nn->layers[i]->weights);
    printf("layer%d outputs\n", i);
    print_mat(nn->layers[i]->outputs);
    printf("\n");
  }
  free_network(nn);
}

/* python api */
static PyObject *py_train(PyObject *self, PyObject* args) {
//void *py_train(PyObject *self, PyObject* args) {
  PyObject *inputs, *loutputs, *linputs, *activations;
  Py_buffer inputsv, loutputsv, linputsv, activationsv;

  // Get the passed Python object
  if (!PyArg_ParseTuple(args, "OOOO", &inputs, &loutputs, &linputs, &activations)) {
    return NULL;
  }

  // Extract buffer info
  if (PyObject_GetBuffer(inputs, &inputsv, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1) {
    return NULL;
  }

  if (PyObject_GetBuffer(loutputs, &loutputsv, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1) {
    return NULL;
  }

  if (PyObject_GetBuffer(linputs, &linputsv, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1) {
    return NULL;
  }

  if (PyObject_GetBuffer(activations, &activationsv, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1) {
    return NULL;
  }

  // Check the type of items in the array
  if (strcmp(inputsv.format, "d") != 0) {
    PyErr_SetString(PyExc_TypeError, "inputs must be doubles");
    PyBuffer_Release(&inputsv);
  }

  if (strcmp(loutputsv.format, "i") != 0) {
    PyErr_SetString(PyExc_TypeError, "outputs must be int");
    PyBuffer_Release(&inputsv);
  }

  if (strcmp(linputsv.format, "i") != 0) {
    PyErr_SetString(PyExc_TypeError, "layer inputs must be int");
    PyBuffer_Release(&inputsv);
  }

  if (strcmp(activationsv.format, "i") != 0) {
    PyErr_SetString(PyExc_TypeError, "activations must be int");
    PyBuffer_Release(&inputsv);
  }

  // Pass the raw buffer and size to the C function
  train(inputsv.buf, inputsv.shape[0], loutputsv.buf, loutputsv.shape[0], linputsv.buf, activationsv.buf);

  PyBuffer_Release(&inputsv);
  PyBuffer_Release(&loutputsv);
  PyBuffer_Release(&linputsv);
  PyBuffer_Release(&inputsv);

  Py_RETURN_NONE;
}

/* Module method table */
static PyMethodDef NetworkMethods[] = {
  {"train", py_train, METH_VARARGS, "trains network"},
  {NULL, NULL, 0, NULL}
};

/* Module structure */
static struct PyModuleDef pynetmodule = {
  PyModuleDef_HEAD_INIT,
  "pynet",
  "pypy",
  -1,
  NetworkMethods
};

/* Module initialization function */
PyMODINIT_FUNC PyInit_pynet(void) {
  return PyModule_Create(&pynetmodule);
}
