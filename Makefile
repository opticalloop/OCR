CC = gcc -Iinclude/

CPPFLAGS = `pkg-config --cflags sdl gtk+-3.0` -MMD -D__NO_INLINE__
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address
LDLFLAGS = -lm -lpthread
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

sudoku:
	make -C Sudoku_Solver

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

test:
	./main ocr src/Imagery/image_01.jpeg -v -S output_1 \
	|| ./main ocr src/Imagery/image_02.jpeg -v -S output_2 \
	|| ./main ocr src/Imagery/image_03.jpeg -v -S output_3 \
	|| ./main ocr src/Imagery/image_04.jpeg -v -S output_4 \
	|| ./main ocr src/Imagery/image_05.jpeg -v -S output_5 \
	|| ./main ocr src/Imagery/image_06.jpeg -v -S output_6

# Clean all trash files
clean:
	make -C $(SOURCE_DIR)/Imagery clean
	make -C $(SOURCE_DIR)/NeuralNetwork clean
	make -C $(SOURCE_DIR)/Sudoku_Solver clean
	make -C Tests clean
	rm -rf $(BUILD)
	${RM} ${OBJ} ${DEP} main
	clear
