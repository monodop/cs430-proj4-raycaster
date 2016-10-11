//
// Created by harrison on 9/17/2016.
//

#ifndef CS430_PROJ1_IMAGES_IMAGE_H
#define CS430_PROJ1_IMAGES_IMAGE_H

#include "color.h"

typedef struct {
    char ppmType;
    unsigned int imageWidth;
    unsigned int imageHeight;
    unsigned short maxVal;
} PpmHeader;
typedef PpmHeader* PpmHeaderRef;

typedef struct {
    PpmHeader header;
    ColorRef pixels;
} PpmImage;
typedef PpmImage* PpmImageRef;

/**
 * Creates a blank image of size width x height and fills it with the provided background color
 * @param image - The place to store the image
 * @param width - The width of the image
 * @param height - The height of the image
 * @param backgroundColor - The color to fill the image with
 * @return 1 if success, 0 if failure
 */
int image_create(PpmImageRef image, unsigned int width, unsigned int height, Color backgroundColor);

void image_fill(PpmImageRef image, Color backgroundColor);

#endif //CS430_PROJ1_IMAGES_IMAGE_H
