//
// Created by harrison on 9/20/2016.
//

#ifndef CS430_PROJ2_RAYCASTER_RAYCASTER_H
#define CS430_PROJ2_RAYCASTER_RAYCASTER_H

#include "scene.h"
#include "image.h"

/**
 * Raycasts from the provided camera into the scene, and stores the image on image.
 * @param image - Where to store the resulting image data
 * @param scene - The scene to render
 * @return 1 if successful, 0 if failure
 */
int raycast_image(PpmImageRef image, SceneRef scene);

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
