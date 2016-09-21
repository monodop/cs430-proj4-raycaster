//
// Created by harrison on 9/20/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_VEC_H
#define CS430_PROJ2_RAYCASTER_VEC_H

#include <math.h>

typedef struct {
    double x;
    double y;
    double z;
} Vector;
typedef Vector* VectorRef;

typedef struct {
    Vector pos;
    Vector dir;
} Ray;
typedef Ray* RayRef;

inline double vec_mag(Vector a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

inline Vector vec_add(Vector a, Vector b) {
    return (Vector) {
            .x = a.x + b.x,
            .y = a.y + b.y,
            .z = a.z + b.z
    };
}

inline Vector vec_sub(Vector a, Vector b) {
    return (Vector) {
            .x = a.x - b.x,
            .y = a.y - b.y,
            .z = a.z - b.z
    };
}

inline Vector vec_scale(Vector a, double scale) {
    return (Vector) {
            .x = a.x * scale,
            .y = a.y * scale,
            .z = a.z * scale
    };
}

inline Vector vec_unit(Vector a) {
    return vec_scale(a, 1.0 / vec_mag(a));
}

inline double vec_dot(Vector a, Vector b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline double vec_angle(Vector a, Vector b) {
    return acos(vec_dot(a, b));
}

inline Vector vec_project(Vector a, Vector b) {
    Vector ua = vec_unit(a);
    return vec_scale(ua, vec_dot(ua, b));
}

inline Vector vec_cross(Vector a, Vector b) {
    return (Vector) {
            .x = a.y*b.z - a.z*b.y,
            .y = a.z*b.x - a.x*b.z,
            .z = a.x*b.y - a.y*b.x
    };
}

#endif //CS430_PROJ2_RAYCASTER_VEC_H
