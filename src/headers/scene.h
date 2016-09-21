//
// Created by harrison on 9/20/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_SCENE_H
#define CS430_PROJ2_RAYCASTER_SCENE_H

#include "vec.h"
#include "image.h"

typedef struct {
    double width;
    double height;
} SceneCamera;
typedef SceneCamera* SceneCameraRef;

typedef struct {
    Color color;
    double radius;
} SceneSphere;
typedef SceneSphere* SceneSphereRef;

typedef struct {
    Color color;
    Vector normal;
} ScenePlane;
typedef ScenePlane* ScenePlaneRef;

typedef enum {
    SCENE_OBJECT_CAMERA,
    SCENE_OBJECT_SPHERE,
    SCENE_OBJECT_PLANE
} SceneObjectType;

typedef struct {
    SceneObjectType type;
    Vector pos;
    union {
        SceneCamera camera;
        SceneSphere sphere;
        ScenePlane plane;
    } data;
} SceneObject;
typedef SceneObject* SceneObjectRef;

typedef struct {
    int objectCount;
    SceneObject camera;
    SceneObjectRef objects;
} Scene;
typedef Scene* SceneRef;

#endif //CS430_PROJ2_RAYCASTER_SCENE_H
