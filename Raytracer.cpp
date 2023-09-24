#include "Raytracer.h"

#define Z_FAR 1000000

void Raytracer::rayTrace(Scene& scene) {
  float iCenter, jCenter;
  vec3 rayDirection;
  int objectIdx;
  for (int i=0; i < width; i++) {
    for (int j=0; j < height; j++) {
      iCenter = i+0.5; jCenter = j+0.5; // Ray is cast through center of pixel

      rayDirection = rayCast(iCenter, jCenter, scene);
      objectIdx = hitTest(scene, rayDirection);
      if (objectIdx != -1)
        setColor(i, height-j, scene.sceneObjects[objectIdx]);
    }
  }
}

vec3 Raytracer::rayCast(float iCenter, float jCenter, Scene& scene) {
  float alpha, beta;

  alpha = tan( (scene.fieldOfViewX / 2) ) * (iCenter - (scene.width/2))/(scene.width/2);
  beta = tan( (scene.fieldOfViewY / 2) ) * ((scene.height/2) - jCenter)/(scene.height/2);

  vec3 w_vec = normalize(scene.eye-scene.center);
  vec3 u_vec = normalize(cross(scene.up, w_vec));
  vec3 v_vec = cross(w_vec, u_vec);

  vec3 ray_direction = normalize( alpha*u_vec + beta*v_vec - w_vec);
  return ray_direction;
}

int Raytracer::hitTest(Scene& scene, vec3 rayDirection) {
  float hitDistance, minHitDistance = Z_FAR;
  int intersectObjectIdx = -1;
  for (int i = 0; i < scene.sceneObjects.size(); i++) {
    auto obj = scene.sceneObjects[i];
    hitDistance = obj->hitTest(scene.eye, rayDirection);
    if (hitDistance > 0 and hitDistance < minHitDistance) {
      minHitDistance = hitDistance;
      intersectObjectIdx = i;
    }
  }
  return intersectObjectIdx;
}

void Raytracer::setColor(int i, int j, shared_ptr<SceneObject> object) {
  auto materialProps = object->getMaterialProperties();
  RGBQUAD color;
  // color.rgbRed = (int) (materialProps.ambient[0] * 255);
  // color.rgbGreen = (int) (materialProps.ambient[1] * 255);
  // color.rgbBlue = (int) (materialProps.ambient[2] * 255);
  color.rgbRed = 255;
  color.rgbGreen = 0;
  color.rgbBlue = 0;
  FreeImage_SetPixelColor(image, i, j, &color);
}

void Raytracer::saveImage(string fname) {
  FreeImage_Save(FIF_PNG, image, fname.c_str(), 0);
}

Raytracer::Raytracer(int w, int h) {
  FreeImage_Initialise();
  int bitsPerPixel = 24;
  width = w;
  height = h;
  image = FreeImage_Allocate(width, height, bitsPerPixel);
}

Raytracer::~Raytracer() {
  FreeImage_DeInitialise();
}
