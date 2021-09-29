CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

TESTS_SUDOKU_OBJ = sudoku_test.o ../Sudoku_Solver/sudoku_solver.o ../Sudoku_Solver/Sudoku_Solved/sudoku_solved.o
TESTS_SUDOKU_DEPEND = sudoku_test.d ../Sudoku_Solver/sudoku_solver.d ../Sudoku_Solver/Sudoku_Solved/sudoku_solved.d

all: sudoku

sudoku: $(TESTS_SUDOKU_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

clean:
	make -C ../Sudoku_Solver clean
	${RM} $(TESTS_SUDOKU_OBJ) $(TESTS_SUDOKU_DEPEND) sudoku