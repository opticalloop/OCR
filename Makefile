CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 
OBJ = main.o
TESTOBJ = Tests/Unit/dummy.o

all: main

main: $(OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

nn:
	make -C NeuralNetwork

utils:
	make -C Imagery

check: $(TESTOBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

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
	${RM} *.o *.d check main