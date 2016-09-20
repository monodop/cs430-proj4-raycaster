//
// Created by Harrison on 9/20/2016.
//

#include "../headers/json.h"

#include <ctype.h>
#include "../headers/helpers.h"

typedef enum {
    false,
    true
} bool;

typedef enum {
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOLEAN,
    JSON_NULL,
    JSON_OBJECT,
    JSON_ARRAY
} JsonElementType;

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

int json_parse(FILE* filePointer, JsonElementRef root) {
    int c;
    JsonElementType type;

    skip_whitespace(filePointer);
    c = getc(filePointer);
    if (c == '{') {
        // Object
        skip_whitespace(filePointer);
    } else if (c == '[') {
        // Array
        skip_whitespace(filePointer);
    } else if (c == '"') {
        // String
    } else if (c >= '0' && c <= '9') {
        // Number
    } else if (c == 't') {
        // True?
    } else if (c == 'f') {
        // False?
    } else if (c == 'n') {
        // Null?
    } else if (c == EOF) {
        // Crap!
        return 0;
    }
    return 1;
}

int json_dispose(JsonElementRef root);