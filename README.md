# OCR
### by opticaloop

Software designed to solve sudoku grid based on a picture.
Work with an OCR so the picture need to be a minimum accurate.
The digit recognization is done thanks to an Neural Network, you can fully train him.

### Dependencies
#### Install SDL 1.2
```sh
Ubuntu/Debian : sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev 
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

#### Graphical interface
```sh
./main ocr gui
```

#### Terminal
```sh
./main ocr <image_path> [options]
OCR options : 
    gui : open graphical interface
    -o <output_path> : specify an output path
    -r <angle> : manually rotate the image by the angle in degree
    -v --verbose : print details of process
    -S <folder> : save all intermediate images in a folder
    -hexa : solve an hexadecimal grid
    --help : print ocr help
./main nn [options]
Options nn :
    -xor <nb_hidden_layer> <nb_node_per_hidden> <nb_epoch> : train the neural network on the xor function epoch time
    -train <nb_hidden_layer> <nb_node_per_hidden> <nb_epoch> :
    train the network with the speficied number of hidden layer and node per hidden layer epoch time
    -reset : reset weights of the neural network (need to train the network after doing that)
    -v --verbose : print details of process
    -L --load <weight_path> : launch the network before trade
    -S --save <weight_path> : save the network after train
    --help : print neural network help
./main data : generate the data in the directory src/NeuralNetwork/Digits-Only/
              If you want to add your own image to train the network on, make sure 
              the last character are : ...<digit repre>
```

#### The entire code is fully documented in the .h files, all in the include folder

# License
MIT License

Copyright (c) 2021 opticalloop

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
