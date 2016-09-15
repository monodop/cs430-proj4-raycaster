//
// Created by harrison on 9/8/2016.
//

#include <stdlib.h>
#include "../headers/ppm_header.h"
#include "../headers/helpers.h"

#define INVALID_FILE_ERROR_MESSAGE "Error: Unable to read PPM headers. The provided file is either not a PPM P3/P6 file or has been corrupted.\n"
#define INVALID_WIDTH_ERROR_MESSAGE "Error: PPM file contained an invalid width.\n"
#define INVALID_HEIGHT_ERROR_MESSAGE "Error: PPM file contained an invalid height.\n"
#define INVALID_MAXVAL_ERROR_MESSAGE "Error: PPM file contained an invalid max value.\n"

int header_read(FILE* filePointer, PpmHeaderRef headerData) {
    int nextChar, count;
    int nextNumber;

    // Check first part of magic number
    nextChar = getc(filePointer);
    if (nextChar != 'P') {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        return 0;
    }

    // Check second part of magic number
    nextChar = getc(filePointer);
    if (nextChar != '3' && nextChar != '6') {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        return 0;
    }

    // Determine which magic number was used
    if (nextChar == '3')
        headerData->ppmType = 3;
    else
        headerData->ppmType = 6;

    // Get the image width
    skip_ignored_characters(filePointer);
    count = fscanf(filePointer, "%d", &nextNumber);
    if (count == 0 || count == EOF || nextNumber <= 0) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_WIDTH_ERROR_MESSAGE);
        return 0;
    }
    headerData->imageWidth = (unsigned int)nextNumber;

    // Get the image height
    skip_ignored_characters(filePointer);
    count = fscanf(filePointer, "%d", &nextNumber);
    if (count == 0 || count == EOF || nextNumber <= 0) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_HEIGHT_ERROR_MESSAGE);
        return 0;
    }
    headerData->imageHeight = (unsigned int)nextNumber;

    // Get the image max value
    skip_ignored_characters(filePointer);
    count = fscanf(filePointer, "%d", &nextNumber);
    if (count == 0 || count == EOF || nextNumber <= 0 || nextNumber > 65535) {
        fprintf(stderr, INVALID_FILE_ERROR_MESSAGE);
        fprintf(stderr, INVALID_MAXVAL_ERROR_MESSAGE);
        return 0;
    }
    headerData->maxVal = (unsigned short)nextNumber;
    return 1;
}

int header_write(FILE* filePointer, PpmHeaderRef headerData) {

    // Write magic number
    fprintf(filePointer, "P%d\n", headerData->ppmType);

    // Write length and width
    fprintf(filePointer, "%u %u\n", headerData->imageWidth, headerData->imageHeight);

    // Write max value
    fprintf(filePointer, "%hu\n", headerData->maxVal);

    return 1;
}