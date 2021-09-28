CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

SUDOKU_OBJ = main.o ../Imagery/Utils/image.o ../Imagery/Utils/pixel_operations.o ../Imagery/Utils/op.o sudoku_solver.o Sudoku_Printer/sudoku_printer.o Sudoku_Saver/sudoku_saver.o Sudoku_Solvable/sudoku_solvable.o Sudoku_Solved/sudoku_solved.o
SUDOKU_DEPEND = main.d ../Imagery/Utils/image.d ../Imagery/Utils/pixel_operations.d ../Imagery/Utils/op. sudoku_solver.d Sudoku_Printer/sudoku_printer.d Sudoku_Saver/sudoku_saver.d Sudoku_Solvable/sudoku_solvable.d Sudoku_Solved/sudoku_solved.d

all: solver

solver: $(SUDOKU_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

clean:
	make -C ../Imagery clean
	${RM} $(SUDOKU_OBJ) $(SUDOKU_DEPEND) solver out.bmp