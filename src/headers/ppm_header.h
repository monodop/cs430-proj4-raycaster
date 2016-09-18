//
// Created by harrison on 9/8/2016.
//

#ifndef CS430_PROJ1_IMAGES_PPM_HEADER_H
#define CS430_PROJ1_IMAGES_PPM_HEADER_H

#include <stdio.h>
#include "image.h"

/**
 * Reads ppm header information from the provided file pointer, and writes the data into the
 * PpmHeader address.
 * @param filePointer - The file pointer of the image file to read from
 * @param headerData - The location of the PpmHeader where the data will be written to
 * @return 1 if success, 0 if failure
 */
int header_read(FILE* filePointer, PpmHeaderRef headerData);

/**
 * Writes ppm header information to the provided file pointer.
 * @param filePointer - The file pointer of the image file to write to
 * @param headerData - The data to write
 * @return 1 if success, 0 if failure
 */
int header_write(FILE* filePointer, PpmHeaderRef headerData);

#endif //CS430_PROJ1_IMAGES_PPM_HEADER_H
