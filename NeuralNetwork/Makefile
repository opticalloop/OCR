CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

XOROBJ = main_XOR.o neural_network.o save_load.o
NNOBJ = main.o neural_network.o save_load.o

all: nn xor

nn: $(NNOBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS)

xor: $(XOROBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS)

clean:
	${RM} *.o *.d xor nn