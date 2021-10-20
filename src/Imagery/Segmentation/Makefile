CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

IMAGE_OBJ = main.o reverse_color.o trace.o struct.o houghtransform.o ../Utils/image.o ../Utils/op.o ../Utils/pixel_operations.o

all: main

main: $(IMAGE_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

clean:
	${RM} *.o *.d imagery out.bmp
