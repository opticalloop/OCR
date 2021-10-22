CC=gcc

CPPFLAGS = `pkg-config --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -O1 -g -fsanitize=address 
LDLFLAGS = -lm
LDLIBS = -lSDL -lpthread -lSDL_image 

BUILD := build

SRC = main.c split81.c ../Imagery/Utils/image.c ../Imagery/Utils/pixel_operations.c ../Imagery/Utils/op.c ../Imagery/Color_Treatment/blackandwhite.c

SEGMENTATION81_DEPEND = $(SRC:%.c=$(BUILD)/%.d)
SEGMENTATION81_OBJ = $(SRC:%.c=$(BUILD)/%.o)

all: init segmentation81

init:
	$(shell mkdir -p $(BUILD))
	$(shell mkdir -p $(SRC:%=$(BUILD)/%))

segmentation81: $(SEGMENTATION81_OBJ)
	gcc -o $@ $(CFLAGS) $^ $(LDLFLAGS) $(LDLIBS)

$(BUILD)/%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C ../Imagery clean
	rm -rf $(BUILD)
	${RM} $(SEGMENTATION81_OBJ) $(SEGMENTATION81_DEPEND) segmentation81 out.bmp
