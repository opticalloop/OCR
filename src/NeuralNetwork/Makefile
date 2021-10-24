CC = gcc -I../../include/

CPPFLAGS = `pkg-config --cflags sdl` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = `pkg-config --libs sdl SDL_image`

BUILD := build

SRC = $(shell find -name "*.c") ../Imagery/Utils/op.c ../Imagery/Utils/pixel_operations.c ../Imagery/Utils/image.c
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init nn clear

clear:
	find . -type d -empty -delete

init: 
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

nn: $(OBJ)
	gcc -o main $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	rm -rf $(BUILD) Imagery main