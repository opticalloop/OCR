# OCR
## by opticaloop

Software designed to solve sudoku grid based on a picture.
Work with an OCR so the picture need to be a minimum accurate.
The digit recognization is done thanks to an Neural Network, you can fully train him.

Dependencies :

```sh
SDL 1.2
GTK 3
```

Usage :

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