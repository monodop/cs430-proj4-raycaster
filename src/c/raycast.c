//
// Created by harrison on 9/20/2016.
//

#include "../headers/raycast.h"
#include "../headers/helpers.h"
#include "../headers/scene.h"

#include <pthread.h>
#include <unistd.h>

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

int raycast_intersect(Ray ray, SceneRef scene, double maxDistance, VectorRef hitPosition, VectorRef hitNormal, SceneObjectRef* hitObject) {

    Vector bestHit = (Vector) { .x = INFINITY, .y = INFINITY, .z = INFINITY };
    Vector bestNormal = (Vector) { .x = 0, .y = 0, .z = 0 };
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
                if (distance < bestDistance && distance <= maxDistance && distance >= 0.001) {
                    bestDistance = distance;
                    bestHit = hit;
                    bestNormal = vec_unit(vec_sub(hit, scene->objects[i].pos));
                    bestHitObject = &(scene->objects[i]);
                }
                break;
            case SCENE_OBJECT_PLANE:
                // Test plane intersection
                plane_intersect(ray, scene->objects[i].pos, scene->objects[i].data.plane.normal, &hit, &distance);
                if (distance < bestDistance && distance <= maxDistance && distance >= 0.001) {
                    bestDistance = distance;
                    bestHit = hit;
                    bestNormal = scene->objects[i].data.plane.normal;
                    bestHitObject = &(scene->objects[i]);
                }
            default:
                // Don't check anything else
                break;
        }
    }

    // Pass back the best hit
    (*hitPosition) = bestHit;
    (*hitNormal) = bestNormal;
    if (bestDistance != INFINITY) {
        (*hitObject) = bestHitObject;
    }

    return 1;
}

Color raycast_calculate_diffuse(Color surfaceColor, Color lightColor, Vector surfaceNormal, Vector lightDirection) {
    Color out = (Color) {.r=0,.g=0,.b=0};
    double dotted = vec_dot(surfaceNormal, lightDirection);

    if (dotted > 0) {
        out.r = dotted * lightColor.r * surfaceColor.r;
        out.g = dotted * lightColor.g * surfaceColor.g;
        out.b = dotted * lightColor.b * surfaceColor.b;
    }

    return out;
}

Color raycast_angular_attenuation(Color lightColor, double aConstant, Vector lightDirection, Vector testDirection) {
    Color out = (Color) {.r=1,.g=1,.b=1};
    double dotted = vec_dot(lightDirection, testDirection);

    if (aConstant == 0)
        return out;
    if (dotted <= 0)
        return (Color) {.r=0,.g=0,.b=0};

    double f = pow(dotted, aConstant);
    out.r = f * lightColor.r;
    out.g = f * lightColor.g;
    out.b = f * lightColor.b;
    return out;
}

int raycast_shoot(Ray ray, SceneRef scene, double maxDistance, ColorRef colorOut) {

    Vector hitPos, hitPos2;
    Vector hitNormal, hitNormal2;
    Vector lightRay, lightDirection;
    SceneObjectRef hitObject, hitObject2;
    Color color = (Color) {.r=0,.g=0,.b=0};
    Color tempColor, lightColor;
    int i;

    // Check for initial hit
    if (!raycast_intersect(ray, scene, maxDistance, &hitPos, &hitNormal, &hitObject)) {
        return 0;
    }
    if (hitPos.x == INFINITY || hitPos.y == INFINITY || hitPos.z == INFINITY) {
        return 1;
    }

    for (i = 0; i < scene->objectCount; i++) {
        if (scene->objects[i].type == SCENE_OBJECT_LIGHT) {

            lightRay = vec_sub(scene->objects[i].pos, hitPos);
            lightDirection = vec_unit(lightRay);

            // Ignore if a shadow
            if (!raycast_intersect((Ray){.pos=hitPos,.dir=lightDirection}, scene, vec_mag(lightRay), &hitPos2, &hitNormal2, &hitObject2)) {
                return 0;
            }
            if (hitPos2.x != INFINITY && hitPos2.y != INFINITY && hitPos2.z != INFINITY) {
                continue;
            }

            lightColor = raycast_angular_attenuation(
                    scene->objects[i].color,
                    scene->objects[i].data.light.angularA0,
                    vec_unit(scene->objects[i].data.light.direction),
                    vec_scale(lightDirection, -1)
            );

            // Calculate diffuse
            tempColor = raycast_calculate_diffuse(hitObject->color, lightColor, hitNormal, lightDirection);
            color.r += tempColor.r;
            color.g += tempColor.g;
            color.b += tempColor.b;
        }
    }

    // Save output color
    *colorOut = color;

    return 1;
}

void* raycast_worker(void* arg) {

    Worker* worker = (Worker*)arg;
    SceneRef scene = worker->args.scene;
    PpmImageRef image = worker->args.image;
    Vector vp_center = {.x = 0, .y = 0, .z = 1};

    double vp_width;
    double vp_height;
    int img_width;
    int img_height;

    double pix_width;
    double pix_height;

    Vector point;
    Ray ray;
    Color pixColor;

    long i;
    int x, y;

    bool shouldKill = false;

    while (1) {

        // Wait for signal
        pthread_mutex_lock(&(worker->signalLock));
        while (worker->signal != WORKER_KILL && worker->signal != WORKER_START) {
            pthread_cond_wait(&(worker->signalCond), &(worker->signalLock));
        }
        if (worker->signal == WORKER_KILL)
            shouldKill = true;
        worker->signal = WORKER_ACKNOWLEDGED;
        pthread_mutex_unlock(&(worker->signalLock));

        // If the worker needs to be killed, break the main loop
        if (shouldKill)
            break;

        // Setup calculations
        vp_width = scene->camera->data.camera.width;
        vp_height = scene->camera->data.camera.height;
        img_width = image->header.imageWidth;
        img_height = image->header.imageHeight;

        pix_width = vp_width / (double) img_width;
        pix_height = vp_height / (double) img_height;

        // Loop over pixels in the image
        for (i = worker->args.startIndex; i < worker->args.endIndex; i++) {

            worker->progress = i - worker->args.startIndex;
            index_to_xwy(i, img_width, &x, &y);

            // Calculate ray target
            point.x = vp_center.x - vp_width / 2.0 + pix_width * (x + 0.5);
            point.y = -(vp_center.y - vp_height / 2.0 + pix_height * (y + 0.5));
            point.z = vp_center.z;

            // Rotate camera
            point = vec_rot(point, scene->camera->angle);

            // Create ray
            ray.pos = scene->camera->pos;
            ray.dir = vec_unit(point);

            // Shoot ray
            if (!raycast_shoot(ray, scene, 100.0, &pixColor)) {
                fprintf(stderr, "Error: Unable to shoot ray at x=%d, y = %d.\n", x, y);
                return 0;
            }

            pixColor.r = clamp(0, 1, pixColor.r);
            pixColor.g = clamp(0, 1, pixColor.g);
            pixColor.b = clamp(0, 1, pixColor.b);

            image->pixels[wxy_to_index(img_width, x, y)] = pixColor;

        }

        worker->completed = 1;

    }

    return NULL;
}

int raycast_create_workers(Worker* workers, PpmImageRef image, SceneRef scene, int threadCount) {
    Worker* worker;

    // Distribute the workload across multiple threads
    long total_workload = image->header.imageWidth * image->header.imageHeight;
    long workload = (long)ceil(total_workload / (float)threadCount);
    int w;

    // Create worker threads
    for (w = 0; w < threadCount; w++) {
        worker = workers + w;
        worker->completed = 0;
        worker->progress = 0;
        worker->signal = WORKER_COMPLETED;
        pthread_mutex_init(&(worker->signalLock), NULL);
        pthread_cond_init(&(worker->signalCond), NULL);
        worker->args.image = image;
        worker->args.scene = scene;
        worker->args.startIndex = w * workload;
        worker->args.endIndex = worker->args.startIndex + workload;
        if (worker->args.endIndex > total_workload)
            worker->args.endIndex = total_workload;
        pthread_create(&(worker->thread_handle), NULL, raycast_worker, worker);
    }

    return 1;
}

int raycast_terminate_workers(Worker* workers, int threadCount) {

    Worker* worker;
    int w;

    for (w = 0; w < threadCount; w++) {
        worker = workers + w;

        // Send kill command
        pthread_mutex_lock(&(worker->signalLock));
        worker->signal = WORKER_KILL;
        pthread_cond_signal(&(worker->signalCond));
        pthread_mutex_unlock(&(worker->signalLock));

        // Wait for the thread to kill
        pthread_join(worker->thread_handle, NULL);
        worker->thread_handle = NULL;
    }

    return 1;
}

int raycast_image(Worker* workers, PpmImageRef image, SceneRef scene, int threadCount) {

    printf("Beginning ray casting.\n");
    Worker* worker;
    int w;
    long total_workload = image->header.imageWidth * image->header.imageHeight;

    // Signal workers to begin
    for (w = 0; w < threadCount; w++) {
        worker = workers + w;
        pthread_mutex_lock(&(worker->signalLock));
        worker->progress = 0;
        worker->completed = 0;
        worker->signal = WORKER_START;
        pthread_cond_signal(&(worker->signalCond));
        pthread_mutex_unlock(&(worker->signalLock));
    }

    // Wait for all workers to complete
    int workers_remaining = threadCount;
    long progress;
    while (workers_remaining > 0) {
        progress = 0;
        for (w = 0; w < threadCount; w++) {
            worker = workers + w;
            progress += worker->progress;
            if (worker->signal == WORKER_ACKNOWLEDGED && worker->completed) {
                // Join completed worker thread
                worker->signal = WORKER_COMPLETED;
                workers_remaining--;
            }
        }
        printf("%d%% rays casted\n", (int)((progress * 100)/total_workload));
        sleep(1);
    }

    printf("100%% rays casted\n");
    printf("Ray casting completed.\n");

    return 1;
}