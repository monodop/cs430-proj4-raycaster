//
// Created by harrison on 9/20/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_RAYCASTER_H
#define CS430_PROJ2_RAYCASTER_RAYCASTER_H

#include "scene.h"
#include "image.h"

typedef enum {
    WORKER_ACKNOWLEDGED, // Worker acknowledged command
    WORKER_COMPLETED,    // Worked completed command
    WORKER_START,        // Tell worker to start
    WORKER_KILL          // Tell worker to terminate
} WorkerSignal;

typedef struct {
    PpmImageRef image;
    SceneRef    scene;
    long        startIndex, endIndex;
} WorkerArgs;

typedef struct {
    pthread_t thread_handle;
    long      progress;
    int       completed;

    pthread_mutex_t signalLock;
    pthread_cond_t  signalCond;
    WorkerSignal signal;

    WorkerArgs args;
} Worker;

/**
 * Create the worker threads that will be raycasting
 * @param workers - The allocated thread pool (non-initialized)
 * @param image - A reference to the output image struct
 * @param scene - A reference to the scene struct
 * @param threadCount - The total number of threads in the pool
 * @return
 */
int raycast_create_workers(Worker* workers, PpmImageRef image, SceneRef scene, int threadCount);

/**
 * Terminate the worker threads that raycasted
 * @param workers - The thread pool
 * @param threadCount - The total number of threads in the pool
 * @return
 */
int raycast_terminate_workers(Worker* workers, int threadCount);

/**
 * Raycasts from the provided camera into the scene, and stores the image on image.
 * @param workers - The thread pool of workers
 * @param image - Where to store the resulting image data
 * @param scene - The scene to render
 * @param threadCount - The total number of threads in the pool
 * @return 1 if successful, 0 if failure
 */
int raycast_image(Worker* workers, PpmImageRef image, SceneRef scene, int threadCount);

/**
 * Shoots a ray at the scene, maxing out at maxDistance. Passes a vector hit position and SceneObjectRef object
 * of where the collision occured. If no collision occured, a member of hitPosition will be INFINITY
 * @param ray - The origin and direction of the ray
 * @param scene - The scene to shoot the ray at
 * @param maxDistance - The maximum distance the ray can shoot
 * @param hitPosition - The position the ray hit
 * @param hitObject - The object the ray hit
 * @return 1 if success, 0 if failure
 */
int raycast_shoot(Ray ray, SceneRef scene, double maxDistance, VectorRef hitPosition, SceneObjectRef* hitObject);

#endif //CS430_PROJ2_RAYCASTER_RAYCASTER_H
