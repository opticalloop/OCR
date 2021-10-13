CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

BUILD := build

SRC = main.c ../Imagery/Utils/image.c ../Imagery/Utils/pixel_operations.c ../Imagery/Utils/op.c sudoku_solver.c Sudoku_Saver/sudoku_saver.c Sudoku_Solvable/sudoku_solvable.c Sudoku_Solved/sudoku_solved.c
SUDOKU_DEPEND = $(SRC:%.c=$(BUILD)/%.d)
SUDOKU_OBJ = $(SRC:%.c=$(BUILD)/%.o)

all: init solver clear

clear:
	find . -type d -empty -delete

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

solver: $(SUDOKU_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C ../Imagery clean
	rm -rf $(BUILD)
	${RM} $(SUDOKU_OBJ) $(SUDOKU_DEPEND) solver out.bmp