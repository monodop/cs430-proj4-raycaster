//
// Created by harrison on 9/20/2016.
//

#include <string.h>
#include "../headers/scene.h"
#include "../headers/interpolate.h"

typedef enum {
    SEMT_BOOL,
    SEMT_DOUBLE,
    SEMT_VECTOR,
    SEMT_COLOR
} SceneObjectMetadataType;

typedef struct {
    SceneObjectMetadataType type;
    char* jsonKeyName;
    bool required;
    bool canAnimate;
    union {
        bool* bool;
        double* d;
        VectorRef vec;
        ColorRef col;
        void* p;
    } value;
    union {
        bool bool;
        double d;
        Vector vec;
        Color col;
    } defaultValue;
    union {
        bool** bool;
        double** d;
        VectorRef* vec;
        ColorRef* col;
        void** p;
    } kfs;
} SceneObjectMetadata;
typedef SceneObjectMetadata* SceneObjectMetadataRef;

int scene_get_object_metadata(SceneObjectRef object, SceneObjectMetadataRef metadata, int metadataCount) {
    int i = 0;

    metadata[i++] = (SceneObjectMetadata) {
            .type = SEMT_VECTOR,
            .jsonKeyName = "position",
            .required = true,
            .canAnimate = true,
            .value.vec = &(object->pos),
            .kfs.vec = &(object->posKfs)
    };
    metadata[i++] = (SceneObjectMetadata) {
            .type = SEMT_COLOR,
            .jsonKeyName = "color",
            .required = true,
            .canAnimate = true,
            .value.col = &(object->color),
            .kfs.col = &(object->colorKfs)
    };

    switch (object->type) {
        case SCENE_OBJECT_CAMERA:
            metadata[0].required = false;
            metadata[0].defaultValue.vec = (Vector){.x=0,.y=0,.z=0};
            metadata[1].required = false;
            metadata[1].defaultValue.col = (Color){.r=0,.g=0,.b=0};
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_DOUBLE,
                    .jsonKeyName = "width",
                    .required = true,
                    .canAnimate = true,
                    .value.d = &(object->data.camera.width),
                    .kfs.d = &(object->data.camera.widthKfs)
            };
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_DOUBLE,
                    .jsonKeyName = "height",
                    .required = true,
                    .canAnimate = true,
                    .value.d = &(object->data.camera.height),
                    .kfs.d = &(object->data.camera.heightKfs)
            };
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_BOOL,
                    .jsonKeyName = "animated",
                    .required = false,
                    .canAnimate = false,
                    .value.bool = &(object->data.camera.animated),
                    .defaultValue.bool = false
            };
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_DOUBLE,
                    .jsonKeyName = "startTime",
                    .required = false,
                    .canAnimate = false,
                    .value.d = &(object->data.camera.startTime),
                    .defaultValue.d = 0
            };
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_DOUBLE,
                    .jsonKeyName = "endTime",
                    .required = false,
                    .canAnimate = false,
                    .value.d = &(object->data.camera.endTime),
                    .defaultValue.d = 0
            };
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_DOUBLE,
                    .jsonKeyName = "frameRate",
                    .required = false,
                    .canAnimate = false,
                    .value.d = &(object->data.camera.frameRate),
                    .defaultValue.d = 1
            };
            break;
        case SCENE_OBJECT_PLANE:
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_VECTOR,
                    .jsonKeyName = "normal",
                    .required = true,
                    .canAnimate = false,
                    .value.vec = &(object->data.plane.normal),
                    .kfs.vec = &(object->data.plane.normalKfs)
            };
            break;
        case SCENE_OBJECT_SPHERE:
            metadata[i++] = (SceneObjectMetadata) {
                    .type = SEMT_DOUBLE,
                    .jsonKeyName = "radius",
                    .required = true,
                    .canAnimate = true,
                    .value.d = &(object->data.sphere.radius),
                    .kfs.d = &(object->data.sphere.radiusKfs)
            };
            break;
    }
    return i;
}

int scene_char_to_object_type(char* typeStr, SceneObjectType* typeOut) {
    if (strcmp(typeStr, "camera") == 0) {
        (*typeOut) = SCENE_OBJECT_CAMERA;
    } else if (strcmp(typeStr, "sphere") == 0) {
        (*typeOut) = SCENE_OBJECT_SPHERE;
    } else if (strcmp(typeStr, "plane") == 0) {
        (*typeOut) = SCENE_OBJECT_PLANE;
    } else {
        fprintf(stderr, "Error: Invalid scene json. Unknown object of type '%s' detected.\n", typeStr);
        return 0;
    }
    return 1;
}

int scene_populate_color(JsonElementRef colorRoot, ColorRef color) {
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

int scene_build_object(JsonElementRef currentElement, SceneRef scene, SceneObjectRef currentObject, char* type,
                       bool* cameraFound, int j) {

    SceneObjectMetadata metadata[32];
    SceneObjectMetadata m;
    JsonElementRef subElement;
    int i, metadataCount;

    if (!scene_char_to_object_type(type, &(currentObject->type))) {
        return 0;
    }

    metadataCount = scene_get_object_metadata(currentObject, metadata, 32);
    for (i = 0; i < metadataCount; i++) {

        m = metadata[i];

        // Check if key exists
        if (json_has_key(currentElement, m.jsonKeyName)) {

            switch (m.type) {
                case SEMT_DOUBLE:
                    // Cast to double
                    if (m.canAnimate) {
                        if (!json_key_as_double(currentElement, m.jsonKeyName, *m.kfs.d + j)) {
                            return 0;
                        }
                    } else {
                        if (!json_key_as_double(currentElement, m.jsonKeyName, m.value.d)) {
                            return 0;
                        }
                    }
                    break;
                case SEMT_BOOL:
                    // Cast to bool
                    if (m.canAnimate) {
                        if (!json_key_as_bool(currentElement, m.jsonKeyName, *m.kfs.bool + j)) {
                            return 0;
                        }
                    } else {
                        if (!json_key_as_bool(currentElement, m.jsonKeyName, m.value.bool)) {
                            return 0;
                        }
                    }
                    break;
                case SEMT_COLOR:
                    // Cast to color
                    if (!json_key(currentElement, m.jsonKeyName, &subElement)) {
                        return 0;
                    }
                    if (m.canAnimate) {
                        if (!scene_populate_color(subElement, *m.kfs.col + j)) {
                            return 0;
                        }
                    } else {
                        if (!scene_populate_color(subElement, m.value.col)) {
                            return 0;
                        }
                    }
                    break;
                case SEMT_VECTOR:
                    // Cast to color
                    if (!json_key(currentElement, m.jsonKeyName, &subElement)) {
                        return 0;
                    }
                    if (m.canAnimate) {
                        if (!scene_populate_vector(subElement, *m.kfs.vec + j)) {
                            return 0;
                        }
                    } else {
                        if (!scene_populate_vector(subElement, m.value.vec)) {
                            return 0;
                        }
                    }
                    break;
            }

        } else if (m.required) {
            // Error - param was required
            fprintf(stderr, "Error: Invalid scene json. Object of type '%s' must have a '%s' property", type, m.jsonKeyName);
            return 0;
        } else {
            // Use default value instead
            switch (m.type) {
                case SEMT_DOUBLE:
                    if (m.canAnimate) {
                        (*m.kfs.d[j]) = m.defaultValue.d;
                    } else {
                        (*m.value.d) = m.defaultValue.d;
                    }
                    break;
                case SEMT_BOOL:
                    if (m.canAnimate) {
                        (*m.kfs.bool[j]) = m.defaultValue.bool;
                    } else {
                        (*m.value.bool) = m.defaultValue.bool;
                    }
                    break;
                case SEMT_COLOR:
                    if (m.canAnimate) {
                        (*m.kfs.col[j]) = m.defaultValue.col;
                    } else {
                        (*m.value.col) = m.defaultValue.col;
                    }
                    break;
                case SEMT_VECTOR:
                    if (m.canAnimate) {
                        (*m.kfs.vec[j]) = m.defaultValue.vec;
                    } else {
                        (*m.value.vec) = m.defaultValue.vec;
                    }
                    break;
            }
        }

    }

    if (currentObject->type == SCENE_OBJECT_CAMERA) {
        if (*cameraFound) {
            fprintf(stderr, "Error: Only one camera may be provided. Unable to proceed.\n");
            return 0;
        }
        *cameraFound = true;
        scene->camera = currentObject;
    }

    return 1;
}

int scene_build_malloc_kfs(SceneObjectRef currentObject, char* type, int tCount) {

    size_t mallocSize;
    SceneObjectMetadata metadata[32];
    SceneObjectMetadata m;
    int i, metadataCount;

    currentObject->tValues = malloc(sizeof(double) * tCount);
    if (currentObject->tValues == NULL) {
        return 0;
    }

    if (!scene_char_to_object_type(type, &(currentObject->type))) {
        return 0;
    }

    metadataCount = scene_get_object_metadata(currentObject, metadata, 32);
    for (i = 0; i < metadataCount; i++) {

        m = metadata[i];

        if (m.canAnimate) {

            switch (m.type) {
                case SEMT_DOUBLE:
                    mallocSize = sizeof(double) * tCount;
                    break;
                case SEMT_BOOL:
                    mallocSize = sizeof(bool) * tCount;
                    break;
                case SEMT_COLOR:
                    mallocSize = sizeof(Color) * tCount;
                    break;
                case SEMT_VECTOR:
                    mallocSize = sizeof(Vector) * tCount;
                    break;
            }

            (*m.kfs.p) = malloc(mallocSize);
            if (m.kfs.p == NULL) {
                return 0;
            }

        }

    }

    return 1;
}

int scene_build(JsonElementRef jsonRoot, SceneRef sceneOut) {

    printf("Beginning building scene.\n");

    JsonElementRef currentElement, subElement;
    int i, j;
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
            sceneOut->objects[i].tCount = currentElement->count;
            if (!scene_build_malloc_kfs(sceneOut->objects + i, type, currentElement->count)) {
                fprintf(stderr, "Error: Unable to allocate enough memory to store animation metadata.\n");
                return 0;
            }
            for (j = 0; j < currentElement->count; j++) {
                if (!json_index(currentElement, j, &subElement)) {
                    return 0;
                }

                if (!json_has_key(subElement, "time")) {
                    fprintf(stderr, "Error: Invalid scene json. A frame must have an associated time.\n");
                    return 0;
                }
                if (!json_key_as_double(subElement, "time", &(sceneOut->objects[i].tValues[j]))) {
                    return 0;
                }

                if (!scene_build_object(subElement, sceneOut, sceneOut->objects + i, type, &cameraFound, j)) {
                    return 0;
                }
            }
        } else {
            sceneOut->objects[i].tCount = 1;
            if (!scene_build_malloc_kfs(sceneOut->objects + i, type, 1)) {
                fprintf(stderr, "Error: Unable to allocate enough memory to store animation metadata.\n");
                return 0;
            }
            if (!scene_build_object(currentElement, sceneOut, sceneOut->objects + i, type, &cameraFound, 0)) {
                return 0;
            }
        }

        if (strcmp(type, "camera") == 0) {
            printf("Object %d/%d of type Camera loaded.\n", i+1, jsonRoot->count);
        } else if (strcmp(type, "sphere") == 0) {
            printf("Object %d/%d of type Sphere loaded.\n", i+1, jsonRoot->count);
        } else if (strcmp(type, "plane") == 0) {
            printf("Object %d/%d of type Plane loaded.\n", i + 1, jsonRoot->count);
        }

    }

    if (!cameraFound) {
        fprintf(stderr, "Error: Invalid scene json. A camera must be provided to render the scene.\n");
        return 0;
    }

    printf("Completed building scene.\n");

    return 1;
}

int scene_prep_frame(SceneRef sceneOut, double t) {
    int i, j, metadataCount;
    SceneObjectRef currentObject;
    SceneObjectMetadata metadata[32];
    SceneObjectMetadata m;

    for (i = 0; i < sceneOut->objectCount; i++) {

        currentObject = sceneOut->objects + i;

        metadataCount = scene_get_object_metadata(currentObject, metadata, 32);

        for (j = 0; j < metadataCount; j++) {

            m = metadata[j];
            if (m.canAnimate) {

                switch (m.type) {
                    case SEMT_DOUBLE:
                        (*m.value.d) = interpolate(
                                INTERPOLATE_LINEAR,
                                currentObject->tCount,
                                currentObject->tValues,
                                (*m.kfs.d),
                                t
                        );
                        break;
                    case SEMT_BOOL:
                        fprintf(stderr, "Error: Boolean values cannot be animated.");
                        return 0;
                    case SEMT_VECTOR:
                        (*m.value.vec) = interpolate_vector(
                                INTERPOLATE_LINEAR,
                                currentObject->tCount,
                                currentObject->tValues,
                                (*m.kfs.vec),
                                t
                        );
                        break;
                    case SEMT_COLOR:
                        (*m.value.col) = interpolate_color(
                                INTERPOLATE_LINEAR,
                                currentObject->tCount,
                                currentObject->tValues,
                                (*m.kfs.col),
                                t
                        );
                        break;
                }

            }

        }
    }

    return 1;
}