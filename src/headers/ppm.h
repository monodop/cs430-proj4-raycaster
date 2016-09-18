//
// Created by harrison on 9/17/2016.
//

#ifndef CS430_PROJ1_IMAGES_PPM_H
#define CS430_PROJ1_IMAGES_PPM_H

#include "image.h"

int ppm_read(char* filename, PpmImageRef image);
int ppm_write(char* filename, PpmImageRef image);

#endif //CS430_PROJ1_IMAGES_PPM_H
