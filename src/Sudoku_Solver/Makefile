CC=gcc -I../../include/

CPPFLAGS = `pkg-config --cflags sdl` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = `pkg-config --libs sdl SDL_image`

BUILD := build

IMG_SRC = $(shell find ../Imagery/Utils -name "*.c" ! -name "main.c")
IMG_OBJ = $(IMG_SRC:%.c=$(BUILD)/%.o)

VERBOSE_SRC = ../verbose.c
VERBOSE_OBJ = $(VERBOSE_SRC:%.c=$(BUILD)/%.o)

SRC = $(shell find -name "*.c")
SUDOKU_DEPEND = $(SRC:%.c=$(BUILD)/%.d)
SUDOKU_OBJ = $(SRC:%.c=$(BUILD)/%.o)

all: init solver clear

clear:
	find . -type d -empty -delete

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%.c=$(BUILD)/%))
	$(shell mkdir -p $(IMG_SRC:%.c=$(BUILD)/%))
	$(shell mkdir -p $(VERBOSE_SRC:%.c=$(BUILD)/%))

solver: $(SUDOKU_OBJ) $(IMG_OBJ) $(VERBOSE_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	rm -rf $(BUILD) Imagery
	${RM} solver *.bmp *.result
	rm -f verbose.d verbose.o