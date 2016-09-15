//
// Created by harrison on 9/8/2016.
//

#include "../headers/ppm6.h"
#include <ctype.h>

#define INVALID_FILE_ERROR_MESSAGE "Error: Unable to read PPM6 data. "
#define UNEXPECTED_EOF "Unexpected EOF.\n"
#define INVALID_COLOR_VALUE "Color value out of bounds (must be above 0 and below the max value as defined in the header).\n"

/**
 * Reads a set of 3 values from the input filestream and stores it in the provided ColorRef
 * @param filePointer - the file pointer to read from
 * @param maxValue - the maximum value a number in the file could be (taken from the header)
 * @param color - the color reference to store the values into
 * @return 1 if success, 0 if failure
 */
int ppm6_read_triple(FILE* filePointer, int maxValue, ColorRef color) {
    int c1, c2;

    // Red
    c1 = getc(filePointer);
    if (c1 == EOF) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, UNEXPECTED_EOF);
        return 0;
    }
    if (maxValue > 255) {
        c2 = getc(filePointer);
        if (c2 == EOF) {
            fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
            fprintf(stderr, UNEXPECTED_EOF);
            return 0;
        }
        c1 = (c1 << 8) + c2;
    }
    if (c1 > maxValue) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_COLOR_VALUE);
        return 0;
    }
    color->r = (double)c1 / maxValue;

    // Green
    c1 = getc(filePointer);
    if (c1 == EOF) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, UNEXPECTED_EOF);
        return 0;
    }
    if (maxValue > 255) {
        c2 = getc(filePointer);
        if (c2 == EOF) {
            fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
            fprintf(stderr, UNEXPECTED_EOF);
            return 0;
        }
        c1 = (c1 << 8) + c2;
    }
    if (c1 > maxValue) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_COLOR_VALUE);
        return 0;
    }
    color->g = (double)c1 / maxValue;

    // Blue
    c1 = getc(filePointer);
    if (c1 == EOF) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, UNEXPECTED_EOF);
        return 0;
    }
    if (maxValue > 255) {
        c2 = getc(filePointer);
        if (c2 == EOF) {
            fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
            fprintf(stderr, UNEXPECTED_EOF);
            return 0;
        }
        c1 = (c1 << 8) + c2;
    }
    if (c1 > maxValue) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_COLOR_VALUE);
        return 0;
    }
    color->b = (double)c1 / maxValue;

    return 1;
}

int ppm6_parse_data(FILE* filePointer, PpmHeaderRef header, ColorRef colorGrid) {
    int i, arrLength = header->imageHeight * header->imageWidth;
    int c;

    // Skip one whitespace character
    c = getc(filePointer);
    if (!isspace(c)) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, "\n");
        return 0;
    }

    // Loop through each position, parse the triple to it
    for (i = 0; i < arrLength; i++) {
        if (!ppm6_read_triple(filePointer, header->maxVal, colorGrid + i)) {
            return 0;
        }
    }

    return 1;
}

int ppm6_write_data(FILE* filePointer, PpmHeaderRef header, ColorRef colorGrid) {
    int i, arrLength = header->imageHeight * header->imageWidth;
    int value;
    char nextByte;

    // Write the color values
    for (i = 0; i < arrLength; i++) {

        // Red
        value = (int)(colorGrid[i].r * header->maxVal);
        if (header->maxVal > 255) {
            nextByte = (char)(value >> 8);
            fwrite(&nextByte, 1, sizeof(nextByte), filePointer);
        }
        nextByte = (char)(value & 0xff);
        fwrite(&nextByte, 1, sizeof(nextByte), filePointer);

        // Green
        value = (int)(colorGrid[i].g * header->maxVal);
        if (header->maxVal > 255) {
            nextByte = (char)(value >> 8);
            fwrite(&nextByte, 1, sizeof(nextByte), filePointer);
        }
        nextByte = (char)(value & 0xff);
        fwrite(&nextByte, 1, sizeof(nextByte), filePointer);

        // Blue
        value = (int)(colorGrid[i].b * header->maxVal);
        if (header->maxVal > 255) {
            nextByte = (char)(value >> 8);
            fwrite(&nextByte, 1, sizeof(nextByte), filePointer);
        }
        nextByte = (char)(value & 0xff);
        fwrite(&nextByte, 1, sizeof(nextByte), filePointer);
    }

    return 1;
}