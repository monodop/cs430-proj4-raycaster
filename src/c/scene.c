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

int scene_build_camera(JsonElementRef currentElement, SceneObjectRef currentObject) {
    currentObject->type = SCENE_OBJECT_CAMERA;

    if (!json_has_key(currentElement, "width")) {
        fprintf(stderr, "Error: Invalid scene json. A camera must have a width.\n");
        return 0;
    }
    if (!json_key_as_double(currentElement, "width", &(currentObject->data.camera.width))) {
        return 0;
    }

    if (!json_has_key(currentElement, "height")) {
        fprintf(stderr, "Error: Invalid scene json. A camera must have a height.\n");
        return 0;
    }
    if (!json_key_as_double(currentElement, "height", &(currentObject->data.camera.height))) {
        return 0;
    }

    if (json_has_key(currentElement, "animated")) {
        if (!json_key_as_bool(currentElement, "animated", &(currentObject->data.camera.animated))) {
            return 0;
        }
    } else {
        currentObject->data.camera.animated = false;
    }
    if (json_has_key(currentElement, "startTime")) {
        if (!json_key_as_double(currentElement, "startTime", &(currentObject->data.camera.startTime))) {
            return 0;
        }
    } else {
        currentObject->data.camera.startTime = 0;
    }
    if (json_has_key(currentElement, "endTime")) {
        if (!json_key_as_double(currentElement, "endTime", &(currentObject->data.camera.endTime))) {
            return 0;
        }
    } else {
        currentObject->data.camera.endTime = 0;
    }
    if (json_has_key(currentElement, "frameRate")) {
        if (!json_key_as_double(currentElement, "frameRate", &(currentObject->data.camera.frameRate))) {
            return 0;
        }
    } else {
        currentObject->data.camera.frameRate = 1;
    }

    return 1;
}

int scene_build_sphere(JsonElementRef currentElement, SceneObjectRef currentObject) {

    JsonElementRef subElement;

    currentObject->type = SCENE_OBJECT_SPHERE;

    if (!json_has_key(currentElement, "color")) {
        fprintf(stderr, "Error: Invalid scene json. A sphere must have a color.\n");
        return 0;
    }
    if (!json_key(currentElement, "color", &subElement)) {
        return 0;
    }
    if (!scene_populate_color(subElement, &(currentObject->color))) {
        return 0;
    }

    if (!json_has_key(currentElement, "position")) {
        fprintf(stderr, "Error: Invalid scene json. A sphere must have a position.\n");
        return 0;
    }
    if (!json_key(currentElement, "position", &subElement)) {
        return 0;
    }
    if (!scene_populate_vector(subElement, &(currentObject->pos))) {
        return 0;
    }

    if (!json_has_key(currentElement, "radius")) {
        fprintf(stderr, "Error: Invalid scene json. A sphere must have a radius.\n");
        return 0;
    }
    if (!json_key_as_double(currentElement, "radius", &(currentObject->data.sphere.radius))) {
        return 0;
    }

    return 1;
}

int scene_build_plane(JsonElementRef currentElement, SceneObjectRef currentObject) {

    JsonElementRef subElement;

    currentObject->type = SCENE_OBJECT_PLANE;

    if (!json_has_key(currentElement, "color")) {
        fprintf(stderr, "Error: Invalid scene json. A plane must have a color.\n");
        return 0;
    }
    if (!json_key(currentElement, "color", &subElement)) {
        return 0;
    }
    if (!scene_populate_color(subElement, &(currentObject->color))) {
        return 0;
    }

    if (!json_has_key(currentElement, "position")) {
        fprintf(stderr, "Error: Invalid scene json. A plane must have a position.\n");
        return 0;
    }
    if (!json_key(currentElement, "position", &subElement)) {
        return 0;
    }
    if (!scene_populate_vector(subElement, &(currentObject->pos))) {
        return 0;
    }

    if (!json_has_key(currentElement, "normal")) {
        fprintf(stderr, "Error: Invalid scene json. A plane must have a normal.\n");
        return 0;
    }
    if (!json_key(currentElement, "normal", &subElement)) {
        return 0;
    }
    if (!scene_populate_vector(subElement, &(currentObject->data.plane.normal))) {
        return 0;
    }

    return 1;
}

int scene_build_object(JsonElementRef currentElement, SceneRef scene, SceneObjectRef currentObject, char* type,
                       bool* cameraFound, int i, int objectCount) {

    if (strcmp(type, "camera") == 0) {
        if (*cameraFound) {
            fprintf(stderr, "Error: Invalid scene json. Only one camera is supported in the scene.\n");
            return 0;
        }
        if (!scene_build_camera(currentElement, currentObject)) {
            return 0;
        }
        scene->camera = currentObject;
        *cameraFound = true;
        printf("Object %d/%d of type Camera loaded.\n", i+1, objectCount);
    } else if (strcmp(type, "sphere") == 0) {
        if (!scene_build_sphere(currentElement, currentObject)) {
            return 0;
        }
        printf("Object %d/%d of type Sphere loaded.\n", i+1, objectCount);
    } else if (strcmp(type, "plane") == 0) {
        if (!scene_build_plane(currentElement, currentObject)) {
            return 0;
        }
        printf("Object %d/%d of type Plane loaded.\n", i+1, objectCount);

    } else {
        fprintf(stderr, "Error: Invalid scene json. Unknown object of type '%s' detected.\n", type);
        return 0;
    }
    return 1;
}

int scene_build(JsonElementRef jsonRoot, SceneRef sceneOut) {

    printf("Beginning building scene.\n");

    JsonElementRef currentElement, subElement;
    int i, j, frameCount;
    char* type;
    bool cameraFound = false;

    if (jsonRoot->type != JSON_ARRAY) {
        fprintf(stderr, "Error: Invalid scene json. Root element must be array.\n");
        return 0;
    }

    sceneOut->objectCount = jsonRoot->count;
    sceneOut->objects = malloc(sizeof(SceneObject) * sceneOut->objectCount);
    if (sceneOut->objects == NULL) {
        fprintf(stderr, "Error: Unable to allocate enough memory to store %d scene objects.\n", sceneOut->objectCount);
        return 0;
    }

    printf("%d objects in scene detected.\n", jsonRoot->count);

    for (i = 0; i < jsonRoot->count; i++) {

        if (!json_index(jsonRoot, i, &currentElement)) {
            return 0;
        }
        if (currentElement->type != JSON_OBJECT) {
            fprintf(stderr, "Error: Invald scene json. Array element #%d must be an object.\n", i);
            return 0;
        }

        if (!json_has_key(currentElement, "type")) {
            fprintf(stderr, "Error: Invalid scene json. All scene objects must have a type.\n");
            return 0;
        }

        if (!json_key_as_string(currentElement, "type", &type)) {
            return 0;
        }

        if (json_has_key(currentElement, "frames")) {
            if (!json_key(currentElement, "frames", &currentElement)) {
                return 0;
            }
            if (currentElement->type != JSON_ARRAY || currentElement->count == 0) {
                fprintf(stderr, "Error: frames must be an array of scene objects.\n");
                return 0;
            }
            for (j = 0; j < currentElement->count; j++) {
                if (!json_index(currentElement, j, &subElement)) {
                    return 0;
                }
                if (!scene_build_object(subElement, sceneOut, sceneOut->objects + i, type, &cameraFound, i, jsonRoot->count)) {
                    return 0;
                }
            }
        } else {
            if (!scene_build_object(currentElement, sceneOut, sceneOut->objects + i, type, &cameraFound, i, jsonRoot->count)) {
                return 0;
            }
        }

    }

    if (!cameraFound) {
        fprintf(stderr, "Error: Invalid scene json. A camera must be provided to render the scene.\n");
        return 0;
    }

    printf("Completed building scene.\n");

    return 1;
}