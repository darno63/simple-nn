SLIB=libnet.so
SLIBP=pytest/pynet.cpython-38-x86_64-linux-gnu.so
EXEC=simnet
OBJDIR=./obj/
SRCDIR=./src/

CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm
COMMON = -I/usr/include/python3.8 -fPIC

OBJ = linalg.o network.o main.o
OBJ2 = linalg.o network.o pynetmodule.o
OBJS = $(addprefix $(OBJDIR), $(OBJ))
OBJS2 = $(addprefix ./obj/, $(OBJ2))

.PHONY: pytest

all: $(SLIB) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SLIB): $(OBJS)
	$(CC) $(CFLAGS) -shared $^ -o $@ $(LDFLAGS)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

#$(OBJS): $(addprefix $(SRCDIR), $(OBJ:.o=.c))
	#$(CC) $(CFLAGS) -c $^ -o $@ $(LDFLAGS)

test: t-neuron.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

pytest: $(SLIBP) 

$(SLIBP): $(OBJS2)
	$(CC) $(COMMON) $(CFLAGS) -shared $^ -o $@ $(LDFLAGS)
	x86_64-linux-gnu-gcc -pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -Wl,-z,relro -g -fwrapv \
		-O2 -Wl,-Bsymbolic-functions -Wl,-z,relro -g -fwrapv -O2 -g -fstack-protector-strong -Wformat -Werror=format-security \
		-Wdate-time -D_FORTIFY_SOURCE=2 $(OBJS2) -o $@
#obj/pynetmodule.o obj/linalg.o obj/network.o

$(OBJS2): $(addprefix $(SRCDIR), $(OBJ2:.o=.c))
	x86_64-linux-gnu-gcc -pthread -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O2 -Wall -g -fstack-protector-strong \
		-Wformat -Werror=format-security -g -fwrapv -O2 -g -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time \
		-D_FORTIFY_SOURCE=2 -fPIC -I/usr/include/python3.8 -c src/pynetmodule.c -o obj/pynetmodule.o

obj:
	mkdir -p obj

clean: 
	rm -f $(EXEC) $(SLIB) $(SLIBP)
	rm -rf obj
