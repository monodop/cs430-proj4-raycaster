//
// Created by harrison on 9/26/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_INTERPOLATE_H
#define CS430_PROJ2_RAYCASTER_INTERPOLATE_H

typedef enum {
    INTERPOLATE_STEPWISE_NEAREST
} InterpolationType;

double interpolate(InterpolationType iType, int tCount, double* tValues, double* yValues, double t);

#endif //CS430_PROJ2_RAYCASTER_INTERPOLATE_H
