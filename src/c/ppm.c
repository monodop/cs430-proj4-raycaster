#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../headers/ppm_header.h"
#include "../headers/ppm.h"
#include "../headers/ppm3.h"
#include "../headers/ppm6.h"

int ppm_read(char* filename, PpmImageRef image) {

    FILE* filePointer;
    PpmHeaderRef fileHeader = &(image->header);
    ColorRef pixelGrid;

    // Open input file
    printf("Processing input file.\n");
    filePointer = fopen(filename, "r");
    if (filePointer == NULL) {
        fprintf(stderr, "Error: File '%s' does not exist or cannot be opened. Error number %d.\n", filename, errno);
        return 0;
    }

    // Read input image file's headers
    if (!header_read(filePointer, fileHeader)) {
        fprintf(stderr, "Error: Unable to continue processing image file.\n");
        return 0;
    }
    printf("Detected file with dimensions %u x %u of type P%d, maxval=%hu.\n",
           fileHeader->imageWidth,
           fileHeader->imageHeight,
           fileHeader->ppmType,
           fileHeader->maxVal);

    // Create the pixel grid in memory
    pixelGrid = malloc(sizeof(Color) * fileHeader->imageWidth * fileHeader->imageHeight);
    if (pixelGrid == NULL) {
        // Unable to allocate the correct amount of memory.
        fprintf(stderr, "Error: Unable to allocate enough memory to read the image file. Cannot proceed.\n");
        return 0;
    }

    // Read the data into the grid
    switch (fileHeader->ppmType) {
        case 3:
            if (!ppm3_parse_data(filePointer, fileHeader, pixelGrid)) {
                fprintf(stderr, "Error: Unable to continue reading the image file.\n");
                return 0;
            }
            break;
        case 6:
            if (!ppm6_parse_data(filePointer, fileHeader, pixelGrid)) {
                fprintf(stderr, "Error: Unable to continue reading the image file.\n");
                return 0;
            }
            break;
        default:
            fprintf(stderr, "Error: Unsupported ppm type (must be P3 or P6), unable to read the image.\n");
            return 0;
    }

    // Check if there is any extra data at the end of the file
    if (getc(filePointer) != EOF) {
        fprintf(stderr, "Warning: extra data detected in file. This data will be ignored.\n");
    }

    // Close input file
    fclose(filePointer);
    printf("Input file processed.\n");

    // Update the image's pixel grid
    free(image->pixels);
    image->pixels = pixelGrid;

    return 1;
}

int ppm_write(char* filename, PpmImageRef image) {

    FILE* filePointer;
    PpmHeaderRef fileHeader = &(image->header);
    ColorRef pixelGrid = image->pixels;

    // Open / Clear output file
    printf("Opening output file for writing.\n");
    filePointer = fopen(filename, "w+");
    if (filePointer == NULL) {
        fprintf(stderr, "Error: File '%s' cannot be opened for writing. Error number %d.\n", filename, errno);
        return 0;
    }

    // Write output image file's headers
    if (!header_write(filePointer, fileHeader)) {
        fprintf(stderr, "Error: Unable to write to image file. Operation aborted.\n");
        return 0;
    }

    // Write the data in the grid to the file
    switch (fileHeader->ppmType) {
        case 3:
            if (!ppm3_write_data(filePointer, fileHeader, pixelGrid)) {
                fprintf(stderr, "Error: Unable to continue writing the image file.\n");
                return 0;
            }
            break;
        case 6:
            if (!ppm6_write_data(filePointer, fileHeader, pixelGrid)) {
                fprintf(stderr, "Error: Unable to continue writing the image file.\n");
                return 0;
            }
            break;
        default:
            fprintf(stderr, "Error: Unsupported output ppm type (must be P3 or P6), unable to write the image.\n");
            return 0;
    }

    // Close output file
    fclose(filePointer);
    printf("Output file processed.\n");

    return 1;
}