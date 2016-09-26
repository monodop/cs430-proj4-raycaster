#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "../headers/json.h"
#include "../headers/scene.h"
#include "../headers/ppm.h"
#include "../headers/raycast.h"

int displayUsage();

int main(int argc, char* argv[]) {

    int width, height, frameCount, i, padding, dotIndex;
    char* jsonFilename;
    char* outputFilename;
    char* frameFilename;
    FILE *filePointer;
    JsonElement rootElement;
    Scene scene;
    PpmImage image;

    // Validate argument count
    if (argc != 5)
        return displayUsage();

    // Validate width parameter
    width = atoi(argv[1]);
    if (width <= 0) {
        fprintf(stderr, "Error: Invalid parameter width = '%s', must be greater than 0.\n", argv[1]);
        return displayUsage();
    }

    // Validate height parameter
    height = atoi(argv[2]);
    if (width <= 0) {
        fprintf(stderr, "Error: Invalid parameter height = '%s', must be greater than 0.\n", argv[2]);
        return displayUsage();
    }

    jsonFilename = argv[3];
    outputFilename = argv[4];

    // Open input file
    printf("Processing input json file.\n");
    filePointer = fopen(jsonFilename, "r");
    if (filePointer == NULL) {
        fprintf(stderr, "Error: File '%s' does not exist or cannot be opened. Error number %d.\n", jsonFilename, errno);
        return displayUsage();
    }

    // Parse scene data
    if (!json_parse(filePointer, &rootElement)) {
        fprintf(stderr, "Error: Unable to parse JSON input file. Render cancelled.\n");
        return displayUsage();
    }

    printf("Input json file processed.\n");
    // Close input file
    fclose(filePointer);

    // Build scene
    if (!scene_build(&rootElement, &scene)) {
        fprintf(stderr, "Error: Unable to build object scene. Render cancelled.\n");
        return displayUsage();
    }

    // Unload json data - its no longer needed
    if (!json_dispose(&rootElement)) {
        fprintf(stderr, "Error: Error disposing json structure. Render cancelled.\n");
        return displayUsage();
    }
    printf("Json data unloaded.\n");

    // Setup image file
    if (!image_create(&image, (unsigned int)width, (unsigned int)height, (Color) { .r = 0, .g = 0, .b = 0 })) {
        fprintf(stderr, "Error: Could not create image buffer. Render cancelled.\n");
        return displayUsage();
    }
    printf("Image buffer created.\n");

    // Calculate number of frames
    if (scene.camera.data.camera.animated) {
        frameCount = (int)ceil((scene.camera.data.camera.endTime - scene.camera.data.camera.startTime) * scene.camera.data.camera.frameRate) + 1;
        if (frameCount < 0) {
            fprintf(stderr, "Error: startTime must be greater than endTime. Render cancelled.\n");
            return 0;
        }

        // Calculate frame filename template
        padding = (int)floor(log10(abs(frameCount))) + 1;
        frameFilename = malloc(sizeof(char) * (strlen(outputFilename) + padding + 2));
        dotIndex = (int)strcspn(outputFilename, ".");
        memcpy(frameFilename, outputFilename, dotIndex);
        strcpy(frameFilename + dotIndex + padding + 1, outputFilename + dotIndex);
        frameFilename[dotIndex] = '-';
    } else {
        frameCount = 1;
        frameFilename = outputFilename;
    }

    for (i = 1; i <= frameCount; i++) {

        printf("Rendering frame %d of %d.\n", i, frameCount);

        if (scene.camera.data.camera.animated) {
            sprintf(frameFilename + dotIndex + 1, "%0*d", padding, i);
            frameFilename[dotIndex + padding + 1] = '.';
        }
        // Perform raycasting
        if (!raycast_image(&image, &scene)) {
            fprintf(stderr, "Error: Unable to raycast the image. Render cancelled.\n");
            return displayUsage();
        }

        // Write image to file
        if (!ppm_write(frameFilename, &image)) {
            fprintf(stderr, "Error: Unable to write the output file. Render cancelled.\n");
            return displayUsage();
        }
    }

    return 0;
}

int displayUsage() {
    fprintf(stderr, "Usage: raycast width height input.json output.ppm\n");
    fprintf(stderr, " - width: The width of the image to generate.\n");
    fprintf(stderr, " - height: The height of the image to generate.\n");
    fprintf(stderr, " - input.json: The name of the json file that contains the scene information.\n");
    fprintf(stderr, " - output.ppm: The name of the ppm file that will be created with the output image.\n");
    return 1;
}