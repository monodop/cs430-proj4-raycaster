//
// Created by harrison on 9/26/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_INTERPOLATE_H
#define CS430_PROJ2_RAYCASTER_INTERPOLATE_H

#include "image.h"
#include "vec.h"

typedef enum {
    INTERPOLATE_STEPWISE_NEAREST,
    INTERPOLATE_LINEAR,
} InterpolationType;

double interpolate(InterpolationType iType, int tCount, double* tValues, double* yValues, double t);
Vector interpolate_vector(InterpolationType iType, int tCount, double* tValues, Vector* yValues, double t);
Color interpolate_color(InterpolationType iType, int tCount, double* tValues, Color* yValues, double t);

#endif //CS430_PROJ2_RAYCASTER_INTERPOLATE_H
