CC = gcc -I../../include/

CPPFLAGS = `pkg-config --cflags sdl` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = `pkg-config --libs sdl SDL_image`

BUILD := build
EXEC := main

SRC =  $(shell find -name "*.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init imagery clear

clear:
	find . -type d -empty -delete

init: 
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

imagery: $(OBJ)
	gcc -o $(EXEC) $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	rm -rf $(BUILD)
	$(RM) $(EXEC)