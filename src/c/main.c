#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "../headers/json.h"
#include "../headers/scene.h"

int displayUsage();

int main(int argc, char* argv[]) {

    int width, height;
    char* jsonFilename;
    char* outputFilename;
    FILE *filePointer;
    JsonElement rootElement;
    Scene scene;

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
    json_parse(filePointer, &rootElement);

    // Build scene
    scene_build(&rootElement, &scene);

    // Unload json data - its no longer needed
    json_dispose(&rootElement);

    printf("Hello world!");
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