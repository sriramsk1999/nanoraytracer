#include "Raytracer.h"
#include <iostream>

#define Z_FAR 1000000

void Raytracer::rayTrace(Scene& scene) {
  float iCenter, jCenter;
  vec3 rayDirection, color;
  int currentDepth = 0;
  for (int i=0; i < width; i++) {
    for (int j=0; j < height; j++) {
      // Convention: Ray is cast through center of pixel
      iCenter = i+0.5; jCenter = j+0.5;
      rayDirection = rayCast(iCenter, jCenter, scene);

      // Recursively raytrace a given ray through the scene
      // accounting for shadows and reflections
      color = recursiveRayTrace(scene, scene.eye, rayDirection,
                                currentDepth);
      // height - j as FreeImage array is inverted
      // origin at bottom left instead of top left
      // height - j -1 to align properly with autograder
      setColor(color, i, height-j-1);
    }
  }
}

vec3 Raytracer::recursiveRayTrace(Scene& scene, vec3 eye,
                                  vec3 rayDirection, int currentDepth) {
  vec3 color(0.,0.,0.);
  if (currentDepth >= maxdepth) return color;

  // Get the id of the object being hit by the ray, and the hitPoint
  auto hitResults = hitTest(scene, eye, rayDirection);
  int objectIdx = hitResults.first;
  vec3 hitPoint = hitResults.second;

  if (objectIdx != -1) {
    // Get colour from ray at a single point
    color = computeColorAtPoint(scene, objectIdx, eye, hitPoint);

    auto object = scene.sceneObjects[objectIdx];
    // Cast reflection ray from hitPoint
    vec3 objectNormal = object->getNorm(hitPoint);
    vec3 directionFromEye = normalize(hitPoint - eye);

    // Reflected ray originates at point of intersection
    vec3 reflectEye = hitPoint;
    vec3 reflectDirection = directionFromEye - 2.0f * objectNormal * dot (directionFromEye, objectNormal);
    // Reflected light is weighted by specularity of object
    vec3 specular = object->getMaterialProperties().specular;
    // Recursively compute light intensity
    return color + specular*recursiveRayTrace(scene, reflectEye,
                                              reflectDirection,
                                              currentDepth+1);
  }
  return color;
}

vec3 Raytracer::computeColorAtPoint(Scene& scene, int objectIdx,
                                    vec3 eye, vec3 hitPoint) {
  vec3 color(0.,0.,0.);
  // Compute light at the current pixel
  auto object = scene.sceneObjects[objectIdx];
  materialProperties materialProps = object->getMaterialProperties();
  vec3 objectNormal = object->getNorm(hitPoint);
  vec3 directionToEye = normalize(eye - hitPoint);
  bool isVisible;

  // Base light
  color += materialProps.ambient + materialProps.emission;
  for (auto l : scene.lights) {
    // Check if light is visible from hitPoint
    // If visible, add the specular and diffuse components
    isVisible = isLightVisible(scene, hitPoint, l->getLightPosition());
    if (isVisible)
      color += l->computeLight(hitPoint, directionToEye, materialProps.diffuse,
                               materialProps.specular, materialProps.shininess,
                               objectNormal);
  }
  return color;
}

vec3 Raytracer::rayCast(float iCenter, float jCenter, Scene& scene) {
  float alpha, beta;

  alpha = tan( (scene.fieldOfViewX / 2) ) * (iCenter - (scene.width/2))/(scene.width/2);
  beta = tan( (scene.fieldOfViewY / 2) ) * ((scene.height/2) - jCenter)/(scene.height/2);

  // Construct coordinate frame based on eye and up vector
  vec3 w_vec = normalize(scene.eye-scene.center);
  vec3 u_vec = normalize(cross(scene.up, w_vec));
  vec3 v_vec = cross(w_vec, u_vec);

  vec3 ray_direction = normalize( alpha*u_vec + beta*v_vec - w_vec);
  return ray_direction;
}

pair<int, vec3> Raytracer::hitTest(Scene& scene, vec3 eye, vec3 rayDirection) {
  float hitDistance, minHitDistance = Z_FAR;
  vec3 hitPoint(0,0,0);
  int intersectObjectIdx = -1;
  // Iterate over objects in scene
  // Find the object first hit by the ray i.e. minimum hit distance
  for (int i = 0; i < scene.sceneObjects.size(); i++) {
    auto obj = scene.sceneObjects[i];
    auto objHitResults = obj->hitTest(eye, rayDirection);
    hitDistance = objHitResults.first;
    if (hitDistance > 0 and hitDistance < minHitDistance) {
      minHitDistance = hitDistance;
      hitPoint = objHitResults.second;
      intersectObjectIdx = i;
    }
  }
  return make_pair(intersectObjectIdx, hitPoint);
}

bool Raytracer::isLightVisible(Scene& scene, vec3 eye, vec3 lightpos) {
  vec3 rayDirection = normalize(lightpos-eye);
  bool isVisible = true;
  // Epsilon to slightly shift source towards destination,
  // to avoid object intersecting with itself
  float epsilon = 0.001;
  eye = eye + epsilon*rayDirection;
  for (auto obj : scene.sceneObjects) {
    auto objHitResults = obj->hitTest(eye, rayDirection);
    float hitDistance = objHitResults.first;
    if (hitDistance > 0) {
      isVisible = false;
      break;
    }
  }
  return isVisible;
}

void Raytracer::setColor(vec3 RGB, int i, int j) {
  RGB = RGB * 255.0f;
  //Clamp at permissible values for image
  RGB = glm::clamp(RGB, 0.f, 255.0f);

  RGBQUAD color;
  color.rgbRed = RGB[0];
  color.rgbGreen = RGB[1];
  color.rgbBlue = RGB[2];
  FreeImage_SetPixelColor(image, i, j, &color);
}

void Raytracer::saveImage() {
  FreeImage_Save(FIF_PNG, image, fname.c_str(), 0);
}

void Raytracer::init(int w, int h, string outputFname,
                     int maximumRayTraceDepth) {
  FreeImage_Initialise();
  int bitsPerPixel = 24;
  width = w;
  height = h;
  image = FreeImage_Allocate(width, height, bitsPerPixel);

  fname = outputFname;
  maxdepth = maximumRayTraceDepth;
}

Raytracer::~Raytracer() {
  FreeImage_DeInitialise();
}
