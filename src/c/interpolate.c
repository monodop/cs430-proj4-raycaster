//
// Created by harrison on 9/26/2016.
//

#include <math.h>
#include "../headers/interpolate.h"

double interpolate_stepwise_nearest(int tCount, double* tValues, double* yValues, double t) {
    int i, prevI = 0, nextI = 0;
    double prevT = tValues[0], nextT = 0;

    if (tCount == 1) {
        return yValues[0];
    }

    for (i = 1; i < tCount; i++) {
        nextI = i;
        nextT = tValues[i];

        if (t >= prevT && t <= nextT)
            break;

        prevT = nextT;
        prevI = nextI;
    }

    if (fabs(t - nextT) < fabs(t - prevT)) {
        return yValues[nextI];
    } else {
        return yValues[prevI];
    }
}

double interpolate_linear(int tCount, double* tValues, double* yValues, double t) {
    int i, prevI = 0, nextI = 0;
    double prevT = tValues[0], nextT = 0;

    if (tCount == 1) {
        return yValues[0];
    }

    if (t <= tValues[0]) {
        return yValues[0];
    } else if (t >= tValues[tCount-1]) {
        return yValues[tCount-1];
    }

    for (i = 1; i < tCount; i++) {
        nextI = i;
        nextT = tValues[i];

        if (t >= prevT && t <= nextT)
            break;

        prevT = nextT;
        prevI = nextI;
    }

    return yValues[prevI] + (t-prevT) * (yValues[nextI] - yValues[prevI]) / (nextT-prevT);
}

double interpolate(InterpolationType iType, int tCount, double* tValues, double* yValues, double t) {

    if (tCount == 0)
        return 0;

    switch (iType) {
        case INTERPOLATE_STEPWISE_NEAREST:
            return interpolate_stepwise_nearest(tCount, tValues, yValues, t);
        case INTERPOLATE_LINEAR:
            return interpolate_linear(tCount, tValues, yValues, t);
    }

    return 0;

}

Vector interpolate_vector(InterpolationType iType, int tCount, double* tValues, Vector* yValues, double t) {
    double x_values[tCount], y_values[tCount], z_values[tCount];
    int i;
    for (i = 0; i < tCount; i++) {
        x_values[i] = yValues[i].x;
        y_values[i] = yValues[i].y;
        z_values[i] = yValues[i].z;
    }

    return (Vector) {
            .x = interpolate(iType, tCount, tValues, x_values, t),
            .y = interpolate(iType, tCount, tValues, y_values, t),
            .z = interpolate(iType, tCount, tValues, z_values, t)
    };
}
Color interpolate_color(InterpolationType iType, int tCount, double* tValues, Color* yValues, double t) {
    double r_values[tCount], g_values[tCount], b_values[tCount];
    int i;
    for (i = 0; i < tCount; i++) {
        r_values[i] = yValues[i].r;
        g_values[i] = yValues[i].g;
        b_values[i] = yValues[i].b;
    }

    return (Color) {
            .r = interpolate(iType, tCount, tValues, r_values, t),
            .g = interpolate(iType, tCount, tValues, g_values, t),
            .b = interpolate(iType, tCount, tValues, b_values, t)
    };
}