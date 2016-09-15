//
// Created by Harrison on 9/8/2016.
//

#ifndef CS430_PROJ1_IMAGES_HELPERS_H
#define CS430_PROJ1_IMAGES_HELPERS_H

#include <stdio.h>

typedef struct {
    double r, g, b;
} Color;

typedef Color* ColorRef;

/**
 * Peeks the next character in the provided stream
 * @param fp - The file stream to peek into
 * @return - The character that was read
 */
int peek(FILE* fp);

/**
 * Scans over whitespace and comments in the provided file stream
 * @param fp - The file being read
 * @return the total number of characters skipped
 */
int skip_ignored_characters(FILE* fp);

/**
 * Converts a width, x, and y coordinate to an index in a 1D array
 * @param width - the width of the grid
 * @param x - the x coordinate
 * @param y - the y coordinate
 * @return - the index in the array
 */
int wxy_to_index(int width, int x, int y);

/**
 * Converts an index (of 1D array) and width (of 2D grid) into x and y coordinates
 * @param index - The index in the 1D array
 * @param width - The width of the 2D grid
 * @param x - The pointer to where the x coordinate should be stored
 * @param y - The pointer to where the y coordinate should be stored
 */
void index_to_xwy(int index, int width, int* x, int* y);

#endif //CS430_PROJ1_IMAGES_HELPERS_H
