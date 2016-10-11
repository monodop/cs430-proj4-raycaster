//
// Created by Harrison on 10/11/2016.
//

#ifndef CS430_PROJ3_RAYCASTER_COLOR_H
#define CS430_PROJ3_RAYCASTER_COLOR_H

typedef struct {
    double r, g, b;
} Color;
typedef Color* ColorRef;

typedef enum {
    BLEND_ADD,
    BLEND_SUBTRACT,
    BLEND_MULTIPLY
} BLEND_MODE;

Color color_blend(Color a, Color b, BLEND_MODE blendMode);
Color color_scale(Color a, double scale);

#endif //CS430_PROJ3_RAYCASTER_COLOR_H
