//
// Created by Harrison on 9/8/2016.
//

#ifndef CS430_PROJ1_IMAGES_PPM3_H
#define CS430_PROJ1_IMAGES_PPM3_H

#include <stdio.h>
#include "ppm_header.h"
#include "helpers.h"

/**
 * Provided a file stream, a loaded PpmHeader, and an allocated color grid of proper size, this function
 * parses the data from the file and loads it into the provided color grid.
 * @param filePointer - The file stream to read from
 * @param header - The header information
 * @param colorGrid - The allocated array of colors that the data will be stored to
 * @return 1 if success, 0 if failure
 */
int ppm3_parse_data(FILE* filePointer, PpmHeaderRef header, ColorRef colorGrid);

/**
 * Provided a file stream, a PpmHeader, and a grid of colors, this function writes the color data to the file.
 * This function does not write the file headers
 * @param filePointer - The file stream to write to
 * @param header - The header information
 * @param colorGrid - The array of colors that will be written
 * @return 1 if success, 0 if failure
 */
int ppm3_write_data(FILE* filePointer, PpmHeaderRef header, ColorRef colorGrid);

#endif //CS430_PROJ1_IMAGES_PPM3_H
