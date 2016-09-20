//
// Created by Harrison on 9/20/2016.
//

#include "../headers/json.h"

#include <string.h>
#include "../headers/helpers.h"

/**
 * Reads characters into the provided string reference parameter until an end double quote is reached
 * @param filePointer - The file to read from
 * @param string - The address of the string to store the value to
 * @return 1 if success, 0 if failure
 */
int json_read_string(FILE* filePointer, char** string) {
    int c, oldSize, size, count;
    char* str;
    char* str2;

    // Allocate the initial string
    count = 0;
    size = 64;
    // TODO: check malloc for null return
    str = malloc(sizeof(char) * size);
    memset(str, '\0', (size_t)size);

    // Iterate until end quote is found
    while (1) {
        c = getc(filePointer);
        if (c == '\\') {
            // TODO: Escape codes
            fprintf(stderr, "Error: Unknown escape code. Unable to proceed parsing json file.");
            return 0;
        } else if (c == '"') {
            // End quote - end loop
            break;
        } else if (c == EOF) {
            // Error
            fprintf(stderr, "Error: Unexpected EOF. Unable to proceed parsing json file.");
            return 0;
        } else {
            // Normal character, write it to the string
            str[count] = (char)c;
            count++;
        }

        // Check if we need more memory
        if (count >= size-1) {
            oldSize = size;
            size += 64;

            // Allocate new string
            // TODO: check malloc for null return
            str2 = malloc(sizeof(char) * size);
            memset(str2, '\0', (size_t)size);

            // Copy the old string to its new address and free the old string
            memcpy(str2, str, (size_t)(oldSize));
            free(str);
            str = str2;
        }
    }

    // Copy the string to the input parameter
    (*string) = str;

    return 1;
}

/**
 * Parses the remaining element as if it is a string.
 * @param filePointer - The file to read the data from
 * @param root - The element to save the data to
 * @return 1 if success, 0 if failure
 */
int json_parse_string(FILE* filePointer, JsonElementRef root) {
    root->type = JSON_STRING;
    json_read_string(filePointer, &(root->data.dataString));
    return 1;
}

int json_parse(FILE* filePointer, JsonElementRef root) {
    int c;

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
        json_parse_string(filePointer, root);
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