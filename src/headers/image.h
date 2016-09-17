//
// Created by harrison on 9/17/2016.
//

#ifndef CS430_PROJ1_IMAGES_IMAGE_H
#define CS430_PROJ1_IMAGES_IMAGE_H

typedef struct {
    double r, g, b;
} Color;
typedef Color* ColorRef;

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

#endif //CS430_PROJ1_IMAGES_IMAGE_H
