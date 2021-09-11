# Simple SDL mini Makefile
CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -g -fsanitize=address
LDFLAGS=
LDLIBS= -lSDL -lpthread -lSDL_image

all: display grayscale rotate main

display: op/op.o display.o
	$(CC) $^ -o $@ $(LDLIBS)
grayscale: Pixel_operations/pixel_operations.o Color_Treatment/grayscale.o op/op.o
	$(CC) $^ -o $@ $(LDLIBS)
blackandwhite: Pixel_operations/pixel_operations.o Color_Treatment/blackandwhite.o op/op.o
	$(CC) $^ -o $@ $(LDLIBS)
rotate: op/op.o Pixel_operations/pixel_operations.o
	$(CC) $^ -o $@ $(LDLIBS)
main: main.o op/op.o Pixel_operations/pixel_operations.o Rotations/Rotations.o display.o Color_Treatment/blackandwhite.o Color_Treatment/grayscale.o Rotations/rotations.o
	$(CC) $^ -o $@ $(LDLIBS)
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o debug

clean:
	${RM} *.o *.d main
                    
# END