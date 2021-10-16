CC = gcc -I../include

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

BUILD := build

SRC = sudoku_test.c ../src/Sudoku_Solver/sudoku_solver.c ../src/Sudoku_Solver/Sudoku_Solved/sudoku_solved.c
TESTS_SUDOKU_OBJ = $(SRC:%.c=$(BUILD)/%.o)
TESTS_SUDOKU_DEPEND = $(SRC:%.c=$(BUILD)/%.d)

all: init sudoku

init: 
    $(shell mkdir -p $(BUILD))
    $(shell mkdir -p $(SRC:%=$(BUILD)/%))

sudoku: $(TESTS_SUDOKU_OBJ)
    gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -rf $(BUILD)
    ${RM} $(TESTS_SUDOKU_OBJ) $(TESTS_SUDOKU_DEPEND) sudoku
