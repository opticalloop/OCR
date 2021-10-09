CC = gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address
LDLFLAGS = -lm
LDLIBS = -lSDL -lpthread -lSDL_image

BUILD := build

SRC = main.c NeuralNetwork/neural_network.c NeuralNetwork/XOR.c
OBJ = $(SRC:%.c=$(BUILD)/%.o)
DEP = $(SRC:%.c=$(BUILD)/%.d)

all: init main clear

clear:
	find . -type d -empty -delete

init: 
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

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

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Format all files
format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;

# Clean all trash files
clean:
	make -C GUI clean
	make -C Imagery clean
	make -C NeuralNetwork clean
	make -C Sudoku_Solver clean
	make -C Tests clean
	rm -rf $(BUILD)
	${RM} ${OBJ} ${DEP} main