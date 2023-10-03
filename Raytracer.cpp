#include "Raytracer.h"
#include <iostream>

#define Z_FAR 1000000

void Raytracer::rayTrace(Scene& scene) {
  float iCenter, jCenter;
  vec3 rayDirection, hitPoint;
  int objectIdx;
  for (int i=0; i < width; i++) {
    for (int j=0; j < height; j++) {
      // Convention: Ray is cast through center of pixel
      iCenter = i+0.5; jCenter = j+0.5;
      rayDirection = rayCast(iCenter, jCenter, scene);
      // Get the id of the object being hit by the ray
      auto hitResults = hitTest(scene, rayDirection);
      objectIdx = hitResults.first;
      hitPoint = hitResults.second;
      if (objectIdx != -1)
        setColor(i, height-j, objectIdx, hitPoint, scene);
    }
  }
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

pair<int, vec3> Raytracer::hitTest(Scene& scene, vec3 rayDirection) {
  float hitDistance, minHitDistance = Z_FAR;
  vec3 hitPoint(0,0,0);
  int intersectObjectIdx = -1;
  // Iterate over objects in scene
  // Find the object first hit by the ray i.e. minimum hit distance
  for (int i = 0; i < scene.sceneObjects.size(); i++) {
    auto obj = scene.sceneObjects[i];
    auto objHitResults = obj->hitTest(scene.eye, rayDirection);
    hitDistance = objHitResults.first;
    if (hitDistance > 0 and hitDistance < minHitDistance) {
      minHitDistance = hitDistance;
      hitPoint = objHitResults.second;
      intersectObjectIdx = i;
    }
  }
  return make_pair(intersectObjectIdx, hitPoint);
}

void Raytracer::setColor(int i, int j, int objectIdx, vec3 hitPoint,
                         Scene& scene) {
  auto object = scene.sceneObjects[objectIdx];
  materialProperties materialProps = object->getMaterialProperties();
  vec3 objectNormal = object->getNorm(hitPoint);

  vec3 directionToEye = normalize(scene.eye - hitPoint);
  vec3 RGB, lightRGB, lightXYZ, directionToLight, halfVector;
  vec3 diffuseLight, specularLight;
  vec3 defaultLightAttenuationCoeff(1,0,0);
  float distanceToLight;
  RGB = materialProps.ambient + materialProps.emission;

  for (int lightIdx = 0; lightIdx < scene.pointLights.size(); lightIdx++) {
    lightXYZ = vec3(scene.pointLights[lightIdx][0], scene.pointLights[lightIdx][1], scene.pointLights[lightIdx][2]);
    lightRGB = vec3(scene.pointLights[lightIdx][3], scene.pointLights[lightIdx][4], scene.pointLights[lightIdx][5]);
    directionToLight = normalize(lightXYZ - hitPoint);
    distanceToLight = length(lightXYZ - hitPoint);
    lightRGB = lightRGB/(defaultLightAttenuationCoeff[0] +
                         defaultLightAttenuationCoeff[1]*distanceToLight +
                         defaultLightAttenuationCoeff[2]*distanceToLight*distanceToLight
                         );
    halfVector = normalize (directionToLight + directionToEye);
    diffuseLight = materialProps.diffuse * max( dot(objectNormal, directionToLight), 0.0f);
    specularLight = materialProps.specular * (float) pow(max( dot(objectNormal, halfVector), 0.0f), materialProps.shininess);
    RGB += lightRGB*(diffuseLight + specularLight);
  }

  for (int lightIdx = 0; lightIdx < scene.directionalLights.size(); lightIdx++) {
    lightXYZ = vec3(scene.directionalLights[lightIdx][0], scene.directionalLights[lightIdx][1], scene.directionalLights[lightIdx][2]);
    lightRGB = vec3(scene.directionalLights[lightIdx][3], scene.directionalLights[lightIdx][4], scene.directionalLights[lightIdx][5]);
    directionToLight = normalize(lightXYZ);
    distanceToLight = -1.;
    lightRGB = lightRGB/(defaultLightAttenuationCoeff[0] +
                         defaultLightAttenuationCoeff[1]*distanceToLight +
                         defaultLightAttenuationCoeff[2]*distanceToLight*distanceToLight
                         );
    halfVector = normalize (directionToLight + directionToEye);
    diffuseLight = materialProps.diffuse * max( dot(objectNormal, directionToLight), 0.0f);
    specularLight = materialProps.specular * (float) pow(max( dot(objectNormal, halfVector), 0.0f), materialProps.shininess);
    RGB += lightRGB*(diffuseLight + specularLight);
  }
  RGB = RGB * 255.0f;

  RGBQUAD color;
  color.rgbRed = RGB[0];
  color.rgbGreen = RGB[1];
  color.rgbBlue = RGB[2];

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
