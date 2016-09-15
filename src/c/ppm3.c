//
// Created by Harrison on 9/8/2016.
//

#include "../headers/ppm3.h"
#include "../headers/ppm_header.h"

#define INVALID_FILE_ERROR_MESSAGE "Error: Unable to read PPM3 data. "
#define UNEXPECTED_EOF "Unexpected EOF.\n"
#define INVALID_COLOR_VALUE "Color value out of bounds (must be above 0 and below the max value as defined in the header).\n"

/**
 * Reads a set of 3 values from the input filestream and stores it in the provided ColorRef
 * @param filePointer - the file pointer to read from
 * @param maxValue - the maximum value a number in the file could be (taken from the header)
 * @param color - the color reference to store the values into
 * @return 1 if success, 0 if failure
 */
int ppm3_read_triple(FILE* filePointer, unsigned short maxValue, ColorRef color) {
    int count;
    int nextValue;

    // Red
    skip_ignored_characters(filePointer);
    count = fscanf(filePointer, "%d", &nextValue);
    if (count == 0 || count == EOF) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, UNEXPECTED_EOF);
        return 0;
    } else if (nextValue < 0 || nextValue > maxValue) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_COLOR_VALUE);
        return 0;
    }
    color->r = (double)nextValue / maxValue;

    // Green
    skip_ignored_characters(filePointer);
    count = fscanf(filePointer, "%d", &nextValue);
    if (count == 0 || count == EOF) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, UNEXPECTED_EOF);
        return 0;
    } else if (nextValue < 0 || nextValue > maxValue) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_COLOR_VALUE);
        return 0;
    }
    color->g = (double)nextValue / maxValue;

    // Blue
    skip_ignored_characters(filePointer);
    count = fscanf(filePointer, "%d", &nextValue);
    if (count == 0 || count == EOF) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, UNEXPECTED_EOF);
        return 0;
    } else if (nextValue < 0 || nextValue > maxValue) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_COLOR_VALUE);
        return 0;
    }
    color->b = (double)nextValue / maxValue;

    return 1;
}

int ppm3_parse_data(FILE* filePointer, PpmHeaderRef header, ColorRef colorGrid) {
    int i, arrLength = header->imageHeight * header->imageWidth;

    // Loop through each position, parse the triple to it
    for (i = 0; i < arrLength; i++) {
        if (!ppm3_read_triple(filePointer, header->maxVal, colorGrid + i)) {
            return 0;
        }
    }

    skip_ignored_characters(filePointer);

    return 1;
}

int ppm3_write_data(FILE* filePointer, PpmHeaderRef header, ColorRef colorGrid) {
    int x = 0, i, arrLength = header->imageHeight * header->imageWidth;

    // Write the color values
    for (i = 0; i < arrLength; i++) {
        fprintf(filePointer, "%d %d %d ",
                (int)(colorGrid[i].r * header->maxVal),
                (int)(colorGrid[i].g * header->maxVal),
                (int)(colorGrid[i].b * header->maxVal)
        );
        x++;
        if (x == header->imageWidth) {
            x = 0;
            fprintf(filePointer, "\n");
        }
    }

    return 1;
}