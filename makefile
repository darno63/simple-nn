CC = gcc

CFLAGS = -Wall -g

neuron: neuron.c
	$(CC) $(CFLAGS) -o neuron neuron.c -lm

test: t-neuron.c
	$(CC) $(CFLAGS) -o t-neuron t-neuron.c -lm

clean: 
	rm neuron
