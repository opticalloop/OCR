CC = gcc -Iinclude/

CPPFLAGS = `pkg-config --cflags sdl gtk+-3.0` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address
LDLFLAGS = -lm
LDLIBS = `pkg-config --libs sdl SDL_image gtk+-3.0` -rdynamic

BUILD := build
SOURCE_DIR := src

SRC = $(shell find $(SOURCE_DIR) -name "*.c" ! -name "main.c")
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init main clear

clear:
	find . -type d -empty -delete

init: 
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%.c=$(BUILD)/%))

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

nn:
	make -C NeuralNetwork nn

sudoku:
	make -C Sudoku_Solver

xor:
	make -C NeuralNetwork xor

utils:
	make -C Imagery

tests:
	make -C Tests

gui:
	make -C GUI

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDLFLAGS) $(CPPFLAGS) $(LDLIBS)

# Format all files
format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;

# Clean all trash files
clean:
	make -C $(SOURCE_DIR)/Imagery clean
	make -C $(SOURCE_DIR)/NeuralNetwork clean
	make -C $(SOURCE_DIR)/Sudoku_Solver clean
	make -C Tests clean
	rm -rf $(BUILD)
	${RM} ${OBJ} ${DEP} main
