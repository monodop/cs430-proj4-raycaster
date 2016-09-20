//
// Created by Harrison on 9/20/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_JSON_H
#define CS430_PROJ2_RAYCASTER_JSON_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    false,
    true
} bool;

/**
 * The possible types of json element values.
 */
typedef enum {
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_OBJECT,
    JSON_ARRAY
} JsonElementType;

/**
 * A node in a json structure
 */
typedef struct _JsonElement {
    JsonElementType type;
    union {
        char*   dataString;
        double  dataNumber;
        bool    dataBool;
        char    dataNull;
        struct _JsonElement* dataElements;
    } data;
    char* keys;
} JsonElement;
typedef JsonElement* JsonElementRef;

/**
 * Parses a json file and stores the data at the passed JsonElement root
 * @param filePointer - The file to read from
 * @param root - The reference to the root element to store the data at
 * @return 1 if success, 0 if failure
 */
int json_parse(FILE* filePointer, JsonElementRef root);

#endif //CS430_PROJ2_RAYCASTER_JSON_H
