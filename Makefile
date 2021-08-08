SLIB=libnet.so
EXEC=simnet
OBJDIR=./obj/
SRCDIR=./src/

CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

OBJ = linalg.o network.o main.o
OBJS = $(addprefix $(OBJDIR), $(OBJ))

all: obj $(SLIB) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SLIB): $(OBJS)
	$(CC) $(CFLAGS) -shared $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

test: t-neuron.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

obj:
	mkdir -p obj

clean: 
	rm $(EXEC) $(SLIB)
