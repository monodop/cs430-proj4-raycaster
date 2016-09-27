//
// Created by harrison on 9/20/2016.
//

#include <stdlib.h>
#include <stdio.h>
#include "../headers/image.h"

int image_create(PpmImageRef image, unsigned int width, unsigned int height, Color backgroundColor) {

    // Setup the header
    image->header.maxVal = 255;
    image->header.ppmType = 6;
    image->header.imageWidth = width;
    image->header.imageHeight = height;
    image->pixels = malloc(sizeof(Color) * width * height);
    if (image->pixels == NULL) {
        fprintf(stderr, "Error: Unable to allocate enough memory to store an image of size %dx%d.\n", width, height);
        return 0;
    }

    image_fill(image, backgroundColor);

    return 1;
}

void image_fill(PpmImageRef image, Color backgroundColor) {

    int i;

    // Set the background color
    for (i = 0; i < image->header.imageWidth * image->header.imageHeight; i++) {
        image->pixels[i] = backgroundColor;
    }
}