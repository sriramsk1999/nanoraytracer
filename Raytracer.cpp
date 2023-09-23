#include "Raytracer.h"

void Raytracer::rayTrace(Scene& scene) {
  float iCenter, jCenter;
  vec3 rayDirection;
  bool isIntersect;
  for (int i=0; i < width; i++) {
    for (int j=0; j < height; j++) {
      iCenter = i+0.5; jCenter = j+0.5; // Ray is cast through center of pixel

      rayDirection = rayCast(iCenter, jCenter, scene);
      isIntersect = checkIntersection(scene, rayDirection);
      if (isIntersect) setRedColor(i, height-j);
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

bool Raytracer::checkIntersection(Scene& scene, vec3 rayDirection) {
  vec3 a, b, c, triNorm, hitPoint;
  float ray2Plane, pointA, pointB, pointC;
  bool isIntersect = false;
  float hitDistance;
  for (auto obj : scene.sceneObjects) {
    hitDistance = obj->hitTest(scene.eye, rayDirection);
    isIntersect = (bool) (hitDistance) or isIntersect;
  }
  return isIntersect;
}

void Raytracer::setRedColor(int i, int j) {
  RGBQUAD color;
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
