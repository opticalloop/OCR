# Simple SDL mini Makefile
CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -g -fsanitize=address
LDFLAGS=
LDLIBS= -lSDL -lpthread -lSDL_image

all: main
main: main.o Utils/image.o Utils/op.o Utils/pixel_operations.o Preprocessing/noise_reduction.o Color_Treatment/blackandwhite.o Color_Treatment/grayscale.o
	$(CC) $^ -o $@ $(LDLIBS)
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	${RM} *.o *.d main test out.bmp
                    
# END
