CC = gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = -lSDL -lSDL_image 

BUILD := build

XOR_SRC = main.c XOR.c neural_network.c save_load.c
XOR_OBJ = $(XOR_SRC:%.c=$(BUILD)/%.o)
XOR_DEP = $(XOR_SRC:%.c=$(BUILD)/%.d)

NN_SRC = main.c training.c neural_network.c save_load.c ../Imagery/Utils/op.c ../Imagery/Utils/pixel_operations.c ../Imagery/Utils/image.c
NN_OBJ = $(NN_SRC:%.c=$(BUILD)/%.o)
NN_DEP = $(NN_SRC:%.c=$(BUILD)/%.d)

all: init nn xor clear

clear:
	find . -type d -empty -delete

init: 
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(XOR_SRC:%=$(BUILD)/%))
	$(shell mkdir -p $(NN_SRC:%=$(BUILD)/%))

nn: $(NN_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

xor: $(XOR_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	rm -rf $(BUILD) Imagery