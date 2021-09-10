# Simple SDL mini Makefile
CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= -lSDL -lpthread -lSDL_image

all: display  grayscale 

display: op/op.o display.o
	$(CC) $^ -o $@ $(LDLIBS)
grayscale: pixel_operations/pixel_operations.o grayscale.o op/op.o
	$(CC) $^ -o $@ $(LDLIBS)
blackandwhite: pixel_operations/pixel_operations.o blackandwhite.o op/op.o
	$(CC) $^ -o $@ $(LDLIBS)
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	${RM} *.o *.d display grayscale blackandwhite
                    
# END