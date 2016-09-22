//
// Created by harrison on 9/20/2016.
//

#include "../headers/raycast.h"
#include "../headers/helpers.h"

/**
 * Tests for a sphere intersection against a ray. Passes the hit position and distance to hitOut and distanceOut.
 * If no hit was detected, then distanceOut will be INIFINITY
 * @param ray - The ray to test
 * @param sphere_center - The center of the sphere
 * @param sphere_radius - The radius of the sphere
 * @param hitOut - The place to store the hit location
 * @param distanceOut - The place to store the hit distance
 */
void sphere_intersect(Ray ray, Vector sphere_center, double sphere_radius, VectorRef hitOut, double* distanceOut) {
    double b, c, t, disc;
    Vector diff = vec_sub(ray.pos, sphere_center);

    // Calculate discriminator
    b = 2 * ( ray.dir.x*diff.x + ray.dir.y*diff.y + ray.dir.z*diff.z );
    c = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z - sphere_radius*sphere_radius;
    disc = b*b - 4*c;

    // No intersection if negative discriminator
    if (disc < 0.0) {
        (*hitOut) = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
        (*distanceOut) = INFINITY;
        return;
    }
    disc = sqrt(disc);

    // Calcualte t-value
    t = (-b - disc) / 2.0;
    if (t < 0.0)
        t = (-b + disc) / 2.0;

    // No intersection if t value is negative (sphere is behind ray)
    if (t < 0.0) {
        (*hitOut) = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
        (*distanceOut) = INFINITY;
        return;
    }

    // Output hit
    (*hitOut) = vec_add(ray.pos, vec_scale(ray.dir, t));
    (*distanceOut) = t;
    return;
}

/**
 * Tests for a plane intersection against a ray. Passes the hit position and distance to hitOut and distanceOut.
 * If no hit was detected, then distanceOut will be INIFINITY
 * @param ray - The ray to test
 * @param plane_center - The center of the plane
 * @param plane_normal - The normal vector of the plane
 * @param hitOut - The place to store the hit location
 * @param distanceOut - The place to store the hit distance
 */
void plane_intersect(Ray ray, Vector plane_center, Vector plane_normal, VectorRef hitOut, double* distanceOut) {
    Vector u_pn = vec_unit(plane_normal);
    double vd = vec_dot(u_pn, ray.dir);

    // Perpendicular - no intersection
    if (fabs(vd) < 0.0001) {
        (*hitOut) = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
        (*distanceOut) = INFINITY;
        return;
    }

    // We can check if vd > 0, meaning the normal is pointing away from the ray, but in this case
    // it probably doesn't matter

    double t = vec_dot(vec_sub(plane_center, ray.pos), u_pn) / vd;

    // Intersect behind origin
    if (t < 0.0) {
        (*hitOut) = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
        (*distanceOut) = INFINITY;
        return;
    }

    // Output hit
    (*hitOut) = vec_add(ray.pos, vec_scale(ray.dir, t));
    (*distanceOut) = t;
    return;
}

int raycast_image(PpmImageRef image, SceneRef scene) {

    printf("Beginning ray casting.\n");

    // Setup calculations
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

    int i = 0, totalPixels = img_width * img_height;

    // Loop over pixels in the image
    for (int y = 0; y < img_height; y++ ) {

        for (int x = 0; x < img_width; x++ ) {

            if (i % 250000 == 0) {
                printf("%d%% rays casted\n", (i*100)/totalPixels);
            }
            i++;

            // Calculate ray target
            point.x = vp_center.x - vp_width/2.0 + pix_width * (x + 0.5);
            point.y = -(vp_center.y - vp_height/2.0 + pix_height * (y + 0.5));
            point.z = vp_center.z;

            // Create ray
            ray.pos = (Vector) { .x = 0, .y = 0, .z = 0 };
            ray.dir = vec_unit(point);

            // Shoot ray
            if (!raycast_shoot(ray, scene, 100.0, &hitPos, &hitObject)) {
                fprintf(stderr, "Error: Unable to shoot ray at x=%d, y = %d.\n", x, y);
                return 0;
            }

            // No hit detected, make no changes this loop
            if (hitPos.x == INFINITY || hitPos.y == INFINITY || hitPos.z == INFINITY) {
                continue;
            }

            // Save hit object's color to the pixel
            image->pixels[wxy_to_index(img_width, x, y)] = hitObject->color;

        }

    }

    printf("100%% rays casted\n");
    printf("Ray casting completed.\n");

    return 1;
}

int raycast_shoot(Ray ray, SceneRef scene, double maxDistance, VectorRef hitPosition, SceneObjectRef* hitObject) {

    Vector bestHit = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
    Vector hit;
    double bestDistance = INFINITY;
    double distance;
    SceneObjectRef bestHitObject = NULL;

    // Iterate over the scene
    for (int i = 0; i < scene->objectCount; i++) {
        switch (scene->objects[i].type) {
            case SCENE_OBJECT_SPHERE:
                // Test sphere intersection
                sphere_intersect(ray, scene->objects[i].pos, scene->objects[i].data.sphere.radius, &hit, &distance);
                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestHit = hit;
                    bestHitObject = &(scene->objects[i]);
                }
                break;
            case SCENE_OBJECT_PLANE:
                // Test plane intersection
                plane_intersect(ray, scene->objects[i].pos, scene->objects[i].data.plane.normal, &hit, &distance);
                if (distance < bestDistance) {
                    bestDistance = distance;
                    bestHit = hit;
                    bestHitObject = &(scene->objects[i]);
                }
            default:
                // Don't check anything else
                break;
        }
    }

    // Pass back the best hit
    (*hitPosition) = bestHit;
    if (bestDistance != INFINITY) {
        (*hitObject) = bestHitObject;
    }

    return 1;
}