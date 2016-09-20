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
    size = 32;
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
            size *= 2;

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
    // TODO: Check failure
    json_read_string(filePointer, &(root->data.dataString));
    return 1;
}

/**
 * Parses an element as if it is a number.
 * @param filePointer - The file to read the data from
 * @param root - The element to save the data to
 * @return 1 if success, 0 if failure
 */
int json_parse_number(FILE* filePointer, JsonElementRef root) {
    root->type = JSON_NUMBER;
    // TODO: Check failure
    fscanf(filePointer, "%lf", &(root->data.dataNumber));
    return 1;
}

/**
 * Parses the remaining element as if it is an array.
 * @param filePointer - The file to read the data from
 * @param root - The element to save the data to
 * @return 1 if success, 0 if failure
 */
int json_parse_array(FILE* filePointer, JsonElementRef root) {
    int c, oldSize, size, count;
    JsonElementRef elements, elements2;

    // Allocate the initial array
    count = 0;
    size = 4;
    // TODO: check malloc for null return
    elements = malloc(sizeof(JsonElement) * size);

    // Iterate until right square bracket
    while (1) {
        skip_whitespace(filePointer);
        c = getc(filePointer);
        if (c == ']') {
            // Right bracket - end loop
            break;
        } else if (c == EOF) {
            // Error
            fprintf(stderr, "Error: Unexpected EOF. Unable to proceed parsing json file.\n");
            return 0;
        } else {
            // Try to read in the next element
            ungetc(c, filePointer);
            json_parse(filePointer, elements + count);
            count++;
            // Look for comma
            skip_whitespace(filePointer);
            c = getc(filePointer);
            if (c == ']') {
                // Rick bracket - end loop
                break;
            }
            if (c != ',') {
                // Error
                fprintf(stderr, "Error: Expected ',' or ']', got '%c'. Cannot proceed.\n", c);
                return 0;
            }
        }

        // Check if we need more memory
        if (count >= size-1) {
            oldSize = size;
            size *= 2;

            // Allocate new string
            // TODO: check malloc for null return
            elements2 = malloc(sizeof(JsonElement) * size);

            // Copy the old string to its new address and free the old string
            memcpy(elements2, elements, (size_t)(oldSize));
            free(elements);
            elements = elements2;
        }
    }

    // Copy the elements to the root element
    root->data.dataElements = elements;
    root->count = count;
    root->type = JSON_ARRAY;

    return 1;
}

/**
 * Parses the remaining element as if it is an object.
 * @param filePointer - The file to read the data from
 * @param root - The element to save the data to
 * @return 1 if success, 0 if failure
 */
int json_parse_object(FILE* filePointer, JsonElementRef root) {
    int c, oldSize, size, count;
    JsonElementRef elements, elements2;
    char* key;
    char** keys;
    char** keys2;

    // Allocate the initial array
    count = 0;
    size = 4;
    // TODO: check malloc for null return
    elements = malloc(sizeof(JsonElement) * size);
    keys = malloc(sizeof(char*) * size);

    // Iterate until right curly bracket
    while (1) {
        skip_whitespace(filePointer);
        c = getc(filePointer);
        if (c == '}') {
            // Right bracket - end loop
            break;
        } else if (c == EOF) {
            // Error
            fprintf(stderr, "Error: Unexpected EOF. Unable to proceed parsing json file.\n");
            return 0;
        } else if (c == '"') {
            // Try to read a string
            json_read_string(filePointer, &key);
            skip_whitespace(filePointer);
            c = getc(filePointer);
            if (c != ':') {
                // Error
                fprintf(stderr, "Error: Expected ':', got '%c'. Cannot proceed.\n", c);
                return 0;
            }
            skip_whitespace(filePointer);
            // Try to read in the next element
            json_parse(filePointer, elements + count);
            keys[count] = key;
            count++;
            // Look for comma
            skip_whitespace(filePointer);
            c = getc(filePointer);
            if (c == '}') {
                // Rick bracket - end loop
                break;
            }
            if (c != ',') {
                // Error
                fprintf(stderr, "Error: Expected ',' or '}', got '%c'. Cannot proceed.\n", c);
                return 0;
            }
        } else {
            // Error
            fprintf(stderr, "Error: Expected string key, got '%c'.\n", c);
            return 0;
        }

        // Check if we need more memory
        if (count >= size-1) {
            oldSize = size;
            size *= 2;

            // Allocate new arrays
            // TODO: check malloc for null return
            elements2 = malloc(sizeof(JsonElement) * size);
            keys2 = malloc(sizeof(char*) * size);

            // Copy the old string to its new address and free the old string
            memcpy(elements2, elements, (size_t)(oldSize));
            memcpy(keys2, keys, (size_t)(oldSize));
            free(elements);
            free(keys);
            elements = elements2;
            keys = keys2;
        }
    }

    // Copy the elements to the root element
    root->data.dataElements = elements;
    root->keys = keys;
    root->count = count;
    root->type = JSON_OBJECT;

    return 1;
}

int json_parse(FILE* filePointer, JsonElementRef root) {
    int c;

    skip_whitespace(filePointer);
    c = getc(filePointer);
    if (c == '{') {
        // Object
        json_parse_object(filePointer, root);
    } else if (c == '[') {
        // Array
        json_parse_array(filePointer, root);
    } else if (c == '"') {
        // String
        json_parse_string(filePointer, root);
    } else if ( (c >= '0' && c <= '9') || c == '-' ) {
        // Number
        ungetc(c, filePointer);
        json_parse_number(filePointer, root);
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