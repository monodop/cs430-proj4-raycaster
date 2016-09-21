//
// Created by harrison on 9/20/2016.
//

#include "../headers/raycast.h"
#include "../headers/helpers.h"
#include "../headers/scene.h"

void sphere_intersect(Ray ray, Vector sphere_center, double sphere_radius, VectorRef hitOut, double* distanceOut) {
    double b, c, t, disc;
    Vector diff = vec_sub(ray.pos, sphere_center);

    b = 2 * ( ray.dir.x*diff.x + ray.dir.y*diff.y + ray.dir.z*diff.z );
    c = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z - sphere_radius*sphere_radius;
    disc = b*b - 4*c;

    if (disc < 0.0) {
        (*hitOut) = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
        (*distanceOut) = INFINITY;
        return;
    }
    disc = sqrt(disc);

    t = (-b - disc) / 2.0;
    if (t < 0.0)
        t = (-b + disc) / 2.0;

    (*hitOut) = vec_add(ray.pos, vec_scale(ray.dir, t));
    (*distanceOut) = t;
    return;
}

int raycast_image(PpmImageRef image, SceneRef scene) {

    Vector vp_center = { .x = 0, .y = 0, .z = 1 };
    double vp_width = scene->camera.data.camera.width;
    double vp_height = scene->camera.data.camera.height;
    int img_width = image->header.imageWidth;
    int img_height = image->header.imageHeight;

    double pix_width = vp_width / (double)img_width;
    double pix_height = vp_height / (double)img_height;

    Vector point, hitPos;
    Ray ray;
    SceneObjectRef hitObject;

    for (int y = 0; y < img_height; y++ ) {

        for (int x = 0; x < img_width; x++ ) {

            point.x = vp_center.x - vp_width/2.0 + pix_width * (x + 0.5);
            point.y = -(vp_center.y - vp_height/2.0 + pix_height * (y + 0.5));
            point.z = vp_center.z;

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

    Vector bestHit = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
    Vector hit;
    double bestDistance = INFINITY;
    double distance;
    SceneObjectRef bestHitObject = NULL;

    for (int i = 0; i < scene->objectCount; i++) {
        switch (scene->objects[i].type) {
            case SCENE_OBJECT_SPHERE:
                sphere_intersect(ray, scene->objects[i].pos, scene->objects[i].data.sphere.radius, &hit, &distance);
                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestHit = hit;
                    bestHitObject = &(scene->objects[i]);
                }
                break;
            default:
                // Don't check anything else
                break;
        }
    }

    (*hitPosition) = bestHit;
    if (bestDistance != INFINITY) {
        (*hitObject) = bestHitObject;
    }

    return 1;
}