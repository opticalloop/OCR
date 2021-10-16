# OCR
### by opticaloop

Software designed to solve sudoku grid based on a picture.
Work with an OCR so the picture need to be a minimum accurate.
The digit recognization is done thanks to an Neural Network, you can fully train him.

### Dependencies
#### Install SDL 1.2
```sh
Ubuntu/Debian : sudo apt-get install libsdl-image1.2-dev
Arch/Manjaro  : sudo pacman -S libsdl-image1.2-dev
NixOS         : sudo nix-env -iA nixos.SDL
```

#### Install GTK 3
```sh
Ubuntu/Debian : sudo apt-get install libgtk-3-dev
Arch/Manjaro  : sudo pacman -S libgtk-3-dev
NixOS         :
```

### Installation

```sh
make
```

### Usage

```sh
./main ocr <image_path> [options]
OCR options : 
    gui : open graphical interface
    -o <output_path> : specify an output path
    -r <angle> : manually rotate the image by the angle in degree
    --help : print ocr help
./main nn [options]
Options nn :
 -xor <nb_hidden_layer> <nb_node_per_hidden> : train the neural network on the xor function
    -train <nb_hidden_layer> <nb_node_per_hidden> : train the network with the speficied number of hidden layer and node per hidden layer
    -reset : reset weights of the neural network (need to train the network after doing that)
    --help : print neural network help
```