CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS =-std=c99 -O1 -g -fsanitize=address
LDLFLAGS = -lcriterion -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

IMAGE_OBJ = main.o Utils/image.o Utils/op.o Utils/pixel_operations.o Color_Treatment/blackandwhite.o Color_Treatment/grayscale.o Resize/resize.o Rotations/rotations.o Preprocessing/noise_reduction.o Utils/array_sort.o

all: imagery

imagery: $(IMAGE_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

clean:
	${RM} *.o *.d imagery out.bmp
	
	${RM} Color_Treatment/*.o
	${RM} Color_Treatment/*.d

	${RM} Resize/*.o
	${RM} Resize/*.d
	
	${RM} Rotations/*.o
	${RM} Rotations/*.d

	${RM} Preprocessing/*.o
	${RM} Preprocessing/*.d

	${RM} Utils/*.o
	${RM} Utils/*.d