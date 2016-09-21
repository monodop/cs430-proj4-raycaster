//
// Created by harrison on 9/20/2016.
//

#include "../headers/raycast.h"
#include "../headers/helpers.h"

int raycast_image(PpmImageRef image, SceneRef scene) {

    Vector vp_center = { .x = 0, .y = 0, .z = 1 };
    double vp_width = scene->camera.data.camera.width;
    double vp_height = scene->camera.data.camera.height;
    int img_width = image->header.imageWidth;
    int img_height = image->header.imageHeight;

    double pix_width = vp_width / (float)img_width;
    double pix_height = vp_height / (float)img_height;

    Vector point, hitPos;
    Ray ray;
    SceneObjectRef hitObject;

    for (int y = 0; y < img_height; y++ ) {

        for (int x = 0; x < img_width; x++ ) {

            point.x = vp_center.x - vp_width/2.0 + pix_width * (x + 0.5);
            point.y = vp_center.y - vp_height/2.0 + pix_height * (y + 0.5);
            point.z = -vp_center.z;

            ray.pos = (Vector) { .x = 0, .y = 0, .z = 0 };
            ray.dir = vec_unit(point);

            raycast_shoot(ray, scene, 100.0, &hitPos, &hitObject);

            if (hitPos.x == INFINITY || hitPos.y == INFINITY || hitPos.z == INFINITY) {
                continue;
            }

            image->pixels[wxy_to_index(img_width, x, y)] = hitObject->color;

        }

    }

    return 1;
}

int raycast_shoot(Ray ray, SceneRef scene, double maxDistance, VectorRef hitPosition, SceneObjectRef* hitObject) {
    (*hitPosition) = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
    return 1;
}