//
// Created by harrison on 9/20/2016.
//

#include <string.h>
#include "../headers/scene.h"

int scene_populate_color(JsonElementRef colorRoot, ColorRef color) {
    int i;
    if (colorRoot->type != JSON_ARRAY) {
        fprintf(stderr, "Error: Invalid color. A color object must be an array.\n");
        return 0;
    }

    if (colorRoot->count > 3) {
        fprintf(stderr, "Warning: Color of size larger than 3 detected. Extra values will be ignored.\n");
    }

    color->r = color->g = color->b = 0;
    if (colorRoot->count > 0)
        json_index_as_double(colorRoot, 0, &color->r);
    if (colorRoot->count > 1)
        json_index_as_double(colorRoot, 1, &color->g);
    if (colorRoot->count > 2)
        json_index_as_double(colorRoot, 2, &color->b);

    return 1;
}

int scene_populate_vector(JsonElementRef vectorRoot, VectorRef vector) {
    int i;
    if (vectorRoot->type != JSON_ARRAY) {
        fprintf(stderr, "Error: Invalid vector. A vector object must be an array.\n");
        return 0;
    }

    if (vectorRoot->count > 3) {
        fprintf(stderr, "Warning: Vector of size larger than 4 detected. Extra values will be ignored.\n");
    }

    vector->x = vector->y = vector->z = 0;
    if (vectorRoot->count > 0)
        json_index_as_double(vectorRoot, 0, &vector->x);
    if (vectorRoot->count > 1)
        json_index_as_double(vectorRoot, 1, &vector->y);
    if (vectorRoot->count > 2)
        json_index_as_double(vectorRoot, 2, &vector->z);

    return 1;
}

int scene_build(JsonElementRef jsonRoot, SceneRef sceneOut) {

    JsonElementRef currentElement, subElement;
    SceneObject currentObject;
    int i, o_i;
    char* type;
    bool cameraFound = false;

    if (jsonRoot->type != JSON_ARRAY) {
        fprintf(stderr, "Error: Invalid scene json. Root element must be array.\n");
        return 0;
    }

    sceneOut->objectCount = jsonRoot->count - 1;
    sceneOut->objects = malloc(sizeof(SceneObject) * sceneOut->objectCount);

    for (i = 0, o_i = 0; i < jsonRoot->count; i++) {

        json_index(jsonRoot, i, &currentElement);
        if (currentElement->type != JSON_OBJECT) {
            fprintf(stderr, "Error: Invald scene json. Array element #%d must be an object.\n", i);
            return 0;
        }

        if (!json_has_key(currentElement, "type")) {
            fprintf(stderr, "Error: Invalid scene json. All scene objects must have a type.\n");
            return 0;
        }

        json_key_as_string(currentElement, "type", &type);
        if (strcmp(type, "camera") == 0) {
            if (cameraFound) {
                fprintf(stderr, "Error: Invalid scene json. Only one camera is supported in the scene.\n");
                return 0;
            }
            currentObject.type = SCENE_OBJECT_CAMERA;

            if (!json_has_key(currentElement, "width")) {
                fprintf(stderr, "Error: Invalid scene json. A camera must have a width.\n");
                return 0;
            }
            json_key_as_double(currentElement, "width", &(currentObject.data.camera.width));

            if (!json_has_key(currentElement, "height")) {
                fprintf(stderr, "Error: Invalid scene json. A camera must have a height.\n");
                return 0;
            }
            json_key_as_double(currentElement, "height", &(currentObject.data.camera.height));

            sceneOut->camera = currentObject;
            cameraFound = true;
            continue;
        } else if (strcmp(type, "sphere") == 0) {
            currentObject.type = SCENE_OBJECT_SPHERE;

            if (!json_has_key(currentElement, "color")) {
                fprintf(stderr, "Error: Invalid scene json. A sphere must have a color.\n");
                return 0;
            }
            json_key(currentElement, "color", &subElement);
            scene_populate_color(subElement, &currentObject.color);

            if (!json_has_key(currentElement, "position")) {
                fprintf(stderr, "Error: Invalid scene json. A sphere must have a position.\n");
                return 0;
            }
            json_key(currentElement, "position", &subElement);
            scene_populate_vector(subElement, &currentObject.pos);

            if (!json_has_key(currentElement, "radius")) {
                fprintf(stderr, "Error: Invalid scene json. A sphere must have a radius.\n");
                return 0;
            }
            json_key_as_double(currentElement, "radius", &currentObject.data.sphere.radius);

        } else if (strcmp(type, "plane") == 0) {
            currentObject.type = SCENE_OBJECT_PLANE;

            if (!json_has_key(currentElement, "color")) {
                fprintf(stderr, "Error: Invalid scene json. A plane must have a color.\n");
                return 0;
            }
            json_key(currentElement, "color", &subElement);
            scene_populate_color(subElement, &currentObject.color);

            if (!json_has_key(currentElement, "position")) {
                fprintf(stderr, "Error: Invalid scene json. A plane must have a position.\n");
                return 0;
            }
            json_key(currentElement, "position", &subElement);
            scene_populate_vector(subElement, &currentObject.pos);

            if (!json_has_key(currentElement, "normal")) {
                fprintf(stderr, "Error: Invalid scene json. A plane must have a normal.\n");
                return 0;
            }
            json_key(currentElement, "normal", &subElement);
            scene_populate_vector(subElement, &currentObject.data.plane.normal);
        } else {
            fprintf(stderr, "Error: Invalid scene json. Unknown object of type '%s' detected.\n", type);
            return 0;
        }

        sceneOut->objects[o_i++] = currentObject;
    }

    if (!cameraFound) {
        fprintf(stderr, "Error: Invalid scene json. A camera must be provided to render the scene.\n");
        return 0;
    }

    return 1;
}