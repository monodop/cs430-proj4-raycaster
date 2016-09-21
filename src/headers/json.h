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
    int    count;
    char** keys;
} JsonElement;
typedef JsonElement* JsonElementRef;

/**
 * Parses a json file and stores the data at the passed JsonElement root
 * @param filePointer - The file to read from
 * @param root - The reference to the root element to store the data at
 * @return 1 if success, 0 if failure
 */
int json_parse(FILE* filePointer, JsonElementRef root);

/**
 * Disposes a json element, and frees any memory its allocated
 * @param root - The root element of the json structure
 * @return 1 if success, 0 if failure
 */
int json_dispose(JsonElementRef root);

/**
 * Checks if a JSON_OBJECT has the specified key
 * @param element - The element to check
 * @param key - The key to search for
 * @return 1 if found, 0 if not found, or the element has no keys
 */
int json_has_key(JsonElementRef element, char* key);

/**
 * Get the child element with the specified key and pass it to elementOut.
 * (Element must be of type JSON_OBJECT).
 * @param element - The element to search in
 * @param key - The key to search for
 * @param elementOut - The place to store the result
 * @return 1 if success, 0 if failure (for example, when the key is not found)
 */
int json_key(JsonElementRef element, char* key, JsonElementRef* elementOut);

/**
 * Get the child element with the specified index and pass it to elementOut.
 * (Element must be of type JSON_OBJECT or JSON_ARRAY).
 * @param element - The element to search in
 * @param index - The index to get
 * @param elementOut - The place to store the result
 * @return 1 if success, 0 if failure (for example, when the index is out of range)
 */
int json_index(JsonElementRef element, int index, JsonElementRef* elementOut);

/**
 * Converts the provided element to a string, and stores it to stringOut
 * @param element - The element to convert
 * @param stringOut - The place to store the result
 * @return 1 if success, 0 if failure
 */
int json_as_string(JsonElementRef element, char** stringOut);

/**
 * Converts the provided element to a double, and stores it to doubleOut
 * @param element - The element to convert
 * @param stringOut - The place to store the result
 * @return 1 if success, 0 if failure
 */
int json_as_double(JsonElementRef element, double* doubleOut);

/**
 * Converts the provided element to an int, and stores it to intOut
 * @param element - The element to convert
 * @param stringOut - The place to store the result
 * @return 1 if success, 0 if failure
 */
int json_as_int(JsonElementRef element, int* intOut);

/**
 * Converts the provided element to a bool, and stores it to boolOut
 * @param element - The element to convert
 * @param stringOut - The place to store the result
 * @return 1 if success, 0 if failure
 */
int json_as_bool(JsonElementRef element, bool* boolOut);

/**
 * Shortcut for calling json_index and then json_as_string.
 * @param element - The element to query
 * @param index - The index of the element to get
 * @param stringOut - The place where the string result will be stored
 * @return 1 if success, 0 if failure
 */
int json_index_as_string(JsonElementRef element, int index, char** stringOut);

/**
 * Shortcut for calling json_index and then json_as_double.
 * @param element - The element to query
 * @param index - The index of the element to get
 * @param stringOut - The place where the double result will be stored
 * @return 1 if success, 0 if failure
 */
int json_index_as_double(JsonElementRef element, int index, double* doubleOut);

/**
 * Shortcut for calling json_index and then json_as_int.
 * @param element - The element to query
 * @param index - The index of the element to get
 * @param stringOut - The place where the int result will be stored
 * @return 1 if success, 0 if failure
 */
int json_index_as_int(JsonElementRef element, int index, int* intOut);

/**
 * Shortcut for calling json_index and then json_as_bool.
 * @param element - The element to query
 * @param index - The index of the element to get
 * @param stringOut - The place where the bool result will be stored
 * @return 1 if success, 0 if failure
 */
int json_index_as_bool(JsonElementRef element, int index, bool* boolOut);

/**
 * Shortcut for calling json_key and then json_as_string.
 * @param element - The element to query
 * @param key - The key to search for
 * @param stringOut - The place where the string result will be stored
 * @return 1 if success, 0 if failure
 */
int json_key_as_string(JsonElementRef element, char* key, char** stringOut);

/**
 * Shortcut for calling json_key and then json_as_double.
 * @param element - The element to query
 * @param key - The key to search for
 * @param stringOut - The place where the double result will be stored
 * @return 1 if success, 0 if failure
 */
int json_key_as_double(JsonElementRef element, char* key, double* doubleOut);

/**
 * Shortcut for calling json_key and then json_as_int.
 * @param element - The element to query
 * @param key - The key to search for
 * @param stringOut - The place where the int result will be stored
 * @return 1 if success, 0 if failure
 */
int json_key_as_int(JsonElementRef element, char* key, int* intOut);

/**
 * Shortcut for calling json_key and then json_as_bool.
 * @param element - The element to query
 * @param key - The key to search for
 * @param stringOut - The place where the bool result will be stored
 * @return 1 if success, 0 if failure
 */
int json_key_as_bool(JsonElementRef element, char* key, bool* boolOut);

#endif //CS430_PROJ2_RAYCASTER_JSON_H
