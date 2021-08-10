import pynet
import array

inputs = array.array('d', [1, 2, 3])
loutputs = array.array('i', [3, 2, 3])
linputs = array.array('i', [3, 3, 2])
acts = array.array('i', [0, 0, 1])
#acts = array.array('B', [ord('r'), ord('r'), ord('s')])
pynet.train(inputs, loutputs, linputs, acts)
