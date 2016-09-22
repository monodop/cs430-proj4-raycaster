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
            fprintf(stderr, "Error: Unknown escape code. Unable to proceed parsing json file.\n");
            return 0;
        } else if (c == '"') {
            // End quote - end loop
            break;
        } else if (c == EOF) {
            // Error
            fprintf(stderr, "Error: Unexpected EOF. Unable to proceed parsing json file.\n");
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
    int c, size, count;
    JsonElementRef elements;

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
        if (count >= size) {
            size *= 2;
            // TODO: check malloc for null return
            elements = realloc(elements, sizeof(JsonElement) * size);
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
    int c, size, count;
    JsonElementRef elements;
    char* key;
    char** keys;

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
        if (count >= size) {
            size *= 2;
            // TODO: check malloc for null return
            elements = realloc(elements, sizeof(JsonElement) * size);
            keys = realloc(keys, sizeof(char*) * size);
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
        if (getc(filePointer) != 'r') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        if (getc(filePointer) != 'u') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        if (getc(filePointer) != 'e') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        root->type = JSON_BOOLEAN;
        root->data.dataBool = true;
    } else if (c == 'f') {
        // False?
        if (getc(filePointer) != 'a') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        if (getc(filePointer) != 'l') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        if (getc(filePointer) != 's') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        if (getc(filePointer) != 'e') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        root->type = JSON_BOOLEAN;
        root->data.dataBool = false;
    } else if (c == 'n') {
        // Null?
        if (getc(filePointer) != 'u') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        if (getc(filePointer) != 'l') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        if (getc(filePointer) != 'l') {
            fprintf(filePointer, "Unexpected symbol detected. Unable to proceed.\n");
            return 0;
        }
        root->type = JSON_NULL;
        root->data.dataNull = 0;
    } else if (c == EOF) {
        // Crap!
        return 0;
    }
    return 1;
}

int json_dispose(JsonElementRef root) {
    int i;
    switch (root->type) {
        case JSON_STRING:
            // Dispose the string
            free(root->data.dataString);
            break;
        case JSON_ARRAY:
            // Dispose each element
            for (i = 0; i < root->count; i++) {
                json_dispose(root->data.dataElements + i);
            }
            // Dispose the list
            free(root->data.dataElements);
            break;
        case JSON_OBJECT:
            // Dispose each element and free each key string
            for (i = 0; i < root->count; i++) {
                json_dispose(root->data.dataElements + i);
                free(root->keys[i]);
            }
            // Dispose the lists
            free(root->data.dataElements);
            free(root->keys);
            break;
        default:
            // Normal elements have no allocations, so we're good
            break;
    }
    return 1;
}

int json_has_key(JsonElementRef element, char* key) {
    int i, foundIndex;

    // Check object type
    if (element->type != JSON_OBJECT) {
        return 0;
    }

    // Search for key
    foundIndex = -1;
    for (i = 0; i < element->count; i++) {
        if (strcmp(key, element->keys[i]) == 0) {
            foundIndex = i;
            break;
        }
    }

    // Return result
    return foundIndex >= 0;
}

int json_key(JsonElementRef element, char* key, JsonElementRef* elementOut) {
    int i, foundIndex;

    // Check object type
    if (element->type != JSON_OBJECT) {
        fprintf(stderr, "Can only call json_key on a json value of type JSON_OBJECT.\n");
        return 0;
    }

    // Search for key
    foundIndex = -1;
    for (i = 0; i < element->count; i++) {
        if (strcmp(key, element->keys[i]) == 0) {
            foundIndex = i;
            break;
        }
    }

    // Error if not found
    if (foundIndex < 0) {
        fprintf(stderr, "Could not find key '%s' in json object.", key);
        return 0;
    }

    // Get element at index
    return json_index(element, foundIndex, elementOut);
}

int json_index(JsonElementRef element, int index, JsonElementRef* elementOut) {

    // Check object type
    if (element->type != JSON_OBJECT && element->type != JSON_ARRAY) {
        fprintf(stderr, "Can only call json_index on a json value of type JSON_OBJECT or JSON_ARRAY.\n");
        return 0;
    }

    // Check if index is in range
    if (index >= element->count) {
        fprintf(stderr, "Index %d out of range of element's values.\n", index);
        return 0;
    }

    // pass the reference to the selected element back through elementOut
    (*elementOut) = &(element->data.dataElements[index]);
    return 1;
}

int json_as_string(JsonElementRef element, char** stringOut) {

    // TODO: Allow more types of objects to be cast to string

    if (element->type != JSON_STRING) {
        fprintf(stderr, "Cannot convert a non JSON_STRING value to a string.\n");
        return 0;
    }

    (*stringOut) = element->data.dataString;
    return 1;
}

int json_as_double(JsonElementRef element, double* doubleOut) {

    // TODO: Allow more types of objects to be cast to double

    if (element->type != JSON_NUMBER) {
        fprintf(stderr, "Cannot convert a non JSON_NUMBER value to a double.\n");
        return 0;
    }

    (*doubleOut) = element->data.dataNumber;
    return 1;
}

int json_as_int(JsonElementRef element, int* intOut) {

    // TODO: Allow more types of objects to be cast to int

    if (element->type != JSON_NUMBER) {
        fprintf(stderr, "Cannot convert a non JSON_NUMBER value to an int.\n");
        return 0;
    }

    (*intOut) = (int)element->data.dataNumber;
    return 1;
}

int json_as_bool(JsonElementRef element, bool* boolOut) {

    // TODO: Allow more types of objects to be cast to bool

    if (element->type != JSON_BOOLEAN) {
        fprintf(stderr, "Cannot convert a non JSON_BOOLEAN value to a bool.\n");
        return 0;
    }

    (*boolOut) = element->data.dataBool;
    return 1;
}

int json_index_as_string(JsonElementRef element, int index, char** stringOut) {
    JsonElementRef subElem;
    if (!json_index(element, index, &subElem)) {
        return 0;
    }
    return json_as_string(subElem, stringOut);
}

int json_index_as_double(JsonElementRef element, int index, double* doubleOut) {
    JsonElementRef subElem;
    if (!json_index(element, index, &subElem)) {
        return 0;
    }
    return json_as_double(subElem, doubleOut);
}

int json_index_as_int(JsonElementRef element, int index, int* intOut) {
    JsonElementRef subElem;
    if (!json_index(element, index, &subElem)) {
        return 0;
    }
    return json_as_int(subElem, intOut);
}

int json_index_as_bool(JsonElementRef element, int index, bool* boolOut) {
    JsonElementRef subElem;
    if (!json_index(element, index, &subElem)) {
        return 0;
    }
    return json_as_bool(subElem, boolOut);
}

int json_key_as_string(JsonElementRef element, char* key, char** stringOut) {
    JsonElementRef subElem;
    if (!json_key(element, key, &subElem)) {
        return 0;
    }
    return json_as_string(subElem, stringOut);
}

int json_key_as_double(JsonElementRef element, char* key, double* doubleOut) {
    JsonElementRef subElem;
    if (!json_key(element, key, &subElem)) {
        return 0;
    }
    return json_as_double(subElem, doubleOut);
}

int json_key_as_int(JsonElementRef element, char* key, int* intOut) {
    JsonElementRef subElem;
    if (!json_key(element, key, &subElem)) {
        return 0;
    }
    return json_as_int(subElem, intOut);
}

int json_key_as_bool(JsonElementRef element, char* key, bool* boolOut) {
    JsonElementRef subElem;
    if (!json_key(element, key, &subElem)) {
        return 0;
    }
    return json_as_bool(subElem, boolOut);
}