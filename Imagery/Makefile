CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

IMAGE_OBJ = main.o Utils/image.o Utils/op.o Utils/pixel_operations.o Color_Treatment/blackandwhite.o Color_Treatment/grayscale.o Resize/resize.o Rotations/rotations.o
IMAGE_DEPEND = main.d Utils/image.d Utils/op.d Utils/pixel_operations.d Color_Treatment/blackandwhite.d Color_Treatment/grayscale.d Resize/resize.d Rotations/rotations.d

all: imagery

imagery: $(IMAGE_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

clean:
	${RM} $(IMAGE_OBJ) $(IMAGE_DEPEND) imagery out.bmp