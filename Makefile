CC = gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address
LDLFLAGS = -lm
LDLIBS = -lSDL -lpthread -lSDL_image
OBJ = main.o

all: main

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ NeuralNetwork/neural_network.c NeuralNetwork/XOR.c $(LDLFLAGS) $(LDLIBS)

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

# Format all files
format:
	find -name "*.[ch]" -exec clang-format --verbose -i {} \;

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean all trash files
clean:
	make -C GUI clean
	make -C Imagery clean
	make -C NeuralNetwork clean
	make -C Sudoku_Solver clean
	make -C Tests clean
	${RM} main.o main.d main