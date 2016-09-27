//
// Created by harrison on 9/20/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_VEC_H
#define CS430_PROJ2_RAYCASTER_VEC_H

#include <math.h>

#define PI 3.14159265359

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

static inline double deg2rad(double deg) {
    return deg * (PI / 180.0);
}

static inline double vec_mag(Vector a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

static inline Vector vec_add(Vector a, Vector b) {
    return (Vector) {
            .x = a.x + b.x,
            .y = a.y + b.y,
            .z = a.z + b.z
    };
}

static inline Vector vec_sub(Vector a, Vector b) {
    return (Vector) {
            .x = a.x - b.x,
            .y = a.y - b.y,
            .z = a.z - b.z
    };
}

static inline Vector vec_scale(Vector a, double scale) {
    return (Vector) {
            .x = a.x * scale,
            .y = a.y * scale,
            .z = a.z * scale
    };
}

static inline Vector vec_unit(Vector a) {
    return vec_scale(a, 1.0 / vec_mag(a));
}

static inline double vec_dot(Vector a, Vector b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

static inline double vec_angle(Vector a, Vector b) {
    return acos(vec_dot(a, b));
}

static inline Vector vec_project(Vector a, Vector b) {
    Vector ua = vec_unit(a);
    return vec_scale(ua, vec_dot(ua, b));
}

static inline Vector vec_cross(Vector a, Vector b) {
    return (Vector) {
            .x = a.y*b.z - a.z*b.y,
            .y = a.z*b.x - a.x*b.z,
            .z = a.x*b.y - a.y*b.x
    };
}

static inline Vector vec_rot(Vector point, Vector angle) {
    // angle in yaw/pitch/roll
    Vector v1, v2, v3, degAngle;

    degAngle = (Vector) {
            .x = deg2rad(angle.x),
            .y = -deg2rad(angle.y),
            .z = -deg2rad(angle.z)
    };

    v1 = (Vector) {
            .x = point.x * cos(degAngle.z) - point.y * sin(degAngle.z),
            .y = point.x * sin(degAngle.z) + point.y * cos(degAngle.z),
            .z = point.z
    };

    v2 = (Vector) {
            .z = v1.z * cos(degAngle.y) - v1.x * sin(degAngle.y),
            .x = v1.z * sin(degAngle.y) + v1.x * cos(degAngle.y),
            .y = v1.y
    };

    v3 = (Vector) {
            .y = v2.y * cos(degAngle.x) - v2.z * sin(degAngle.x),
            .z = v2.y * sin(degAngle.x) + v2.z * cos(degAngle.x),
            .x = v2.x
    };

    return v3;
    
}

#endif //CS430_PROJ2_RAYCASTER_VEC_H
