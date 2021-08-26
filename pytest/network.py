import pynet
import array

class Layer:
    weights = []
    biases = []
    def __init__(self, input_shape: tuple, activation: str):
        self.input_shape = input_shape
        self.activation = activation

class layers:
    @staticmethod
    def Dense(input_shape: tuple, activation: str):
        if not isinstance(input_shape, tuple) or len(input_shape) != 2:
            raise TypeError
        elif not isinstance(activation, str):
            raise TypeError
        else:
            return Layer(input_shape, activation)

class Model:
    def __init__(self, layers: list = []):
        self.layers = layers
        self.nlayers = len(layers)

    def train(self, inputs):
        model_inputs = array.array('d', inputs)
        loutputs = array.array('i', [])
        linputs = array.array('i', [])
        acts = array.array('i', [])
        for i in range(self.nlayers):
            loutputs.append(self.layers[i].input_shape[0])
            linputs.append(self.layers[i].input_shape[1])
            act = 1 if self.layers[i].activation == 'sigmoid' else 0
            acts.append(act)
        pynet.train(model_inputs, loutputs, linputs, acts)

if __name__ == "__main__":
    mlayers = [
        layers.Dense((3, 3), 'relu'),
        layers.Dense((2, 3), 'relu'),
        layers.Dense((3, 2), 'sigmoid')
    ]
    nn = Model(mlayers)
    nn.train([1, 2, 3])
