CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

XOR_OBJ = main_XOR.o neural_network.o save_load.o
XOR_DEPEND = main_XOR.d neural_network.d save_load.d
NN_OBJ = main.o neural_network.o save_load.o ../Imagery/Utils/image.o ../Imagery/Utils/pixel_operations.o ../Imagery/Utils/op.o
NN_DEPEND = main.d neural_network.d save_load.d ../Imagery/Utils/image.d ../Imagery/Utils/pixel_operations.d ../Imagery/Utils/op.d

all: nn xor

nn: $(NN_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

xor: $(XOR_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS)

clean:
	${RM} $(NN_OBJ) $(NN_DEPEND) $(XOR_OBJ) $(XOR_DEPEND) xor nn