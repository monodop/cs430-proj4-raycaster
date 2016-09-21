//
// Created by harrison on 9/20/2016.
//

#include <stdlib.h>
#include "../headers/image.h"

int image_create(PpmImageRef image, unsigned int width, unsigned int height, Color backgroundColor) {

    int i;

    // Setup the header
    image->header.maxVal = 255;
    image->header.ppmType = 6;
    image->header.imageWidth = width;
    image->header.imageHeight = height;
    image->pixels = malloc(sizeof(Color) * width * height);
    // TODO: check for null pointer from malloc

    // Set the background color
    for (i = 0; i < width * height; i++) {
        image->pixels[i] = backgroundColor;
    }

    return 1;
}