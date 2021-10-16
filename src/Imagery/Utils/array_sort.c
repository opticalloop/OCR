//
// Created by dov on 11/09/2021.
//

#include "Imagery/Utils/array_sort.h"

void array_sort(Pixel *arr,int n) {
// considering the picture is at least a true grayscale.

    int i, j, min_idx;

    for (i = 0; i < n - 1; i++) {

        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j].b < arr[min_idx].b)
                min_idx = j;


        Pixel temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}