CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

SUDOKU_OBJ = main.o sudoku_solver.o Sudoku_Printer/sudoku_printer.o Sudoku_Saver/sudoku_saver.o Sudoku_Solvable/sudoku_solvable.o Sudoku_Solved/sudoku_solved.o

all: sudoku

sudoku: $(SUDOKU_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

clean:
	${RM} *.o *.d sudoku

	${RM} Sudoku_Printer/*.o
	${RM} Sudoku_Printer/*.d

	${RM} Sudoku_Saver/*.o
	${RM} Sudoku_Saver/*.d

	${RM} Sudoku_Solvable/*.o
	${RM} Sudoku_Solvable/*.d

	${RM} Sudoku_Solved/*.o
	${RM} Sudoku_Solved/*.d