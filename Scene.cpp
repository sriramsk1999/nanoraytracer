#include "Scene.h"
#include <iostream>
#include <stdexcept>
#include <memory>

void Scene::addCamera(vec3 eye, vec3 center, vec3 up, float fovy) {
  Scene::eye = eye;
  Scene::center = center;
  Scene::up = up;
  Scene::fieldOfViewY = glm::radians(fovy);
  Scene::fieldOfViewX = (2 * atan( tan(fieldOfViewY / 2) * ((float) width/ height)));
}

void Scene::setImageResolution(int w, int h) {
  Scene::width = w;
  Scene::height = h;
}

void Scene::addLights(std::string lightType, std::vector<float>& lightParams) {
  if (lightType == "directional") directionalLights.push_back(lightParams);
  else if (lightType == "point") pointLights.push_back(lightParams);
  else throw std::invalid_argument( "invalid lightType: " + lightType );
}

void Scene::addObjectToScene(std::shared_ptr<SceneObject> sceneObj) {
  sceneObjects.push_back(sceneObj);
}

void Triangle::printInfo() {
  std::cout <<
    "Object Type : Triangle\n\
    Vertex A: " << a[0] << " " << a[1] << " " << a[2] << "\n\
    Vertex B: " << b[0] << " " << b[1] << " " << b[2] << "\n\
    Vertex C: " << c[0] << " " << c[1] << " " << c[2] << "\n\
    Ambient: " << materialProps.ambient[0] << " " << materialProps.ambient[1] << " " << materialProps.ambient[2] << "\n\
    Diffuse: " << materialProps.diffuse[0] << " " << materialProps.diffuse[1] << " " << materialProps.diffuse[2] << "\n\
    Specular: " << materialProps.specular[0] << " " << materialProps.specular[1] << " " << materialProps.specular[2] << "\n\
    Emissive: " << materialProps.emission[0] << " " << materialProps.emission[1] << " " << materialProps.emission[2] << "\n\
    Shininess: " << materialProps.shininess << "\n";
}

float Triangle::hitTest(vec3& eye, vec3& rayDirection) {
  vec3 triNorm = normalize( cross(c-a, b-a) );
  vec3 hitPoint, pointA, pointB, pointC;
  float ray2Plane, hitDistance=0.;

  ray2Plane = ( dot(a, triNorm) - dot(eye, triNorm) ) / dot( rayDirection, triNorm );
  hitPoint = eye + rayDirection*ray2Plane;

  pointA = cross(b-a, hitPoint-a);
  pointB = cross(c-b, hitPoint-b);
  pointC = cross(a-c, hitPoint-c);

  if ((pointA[0]>0 and pointB[0]>0 and pointC[0]>0) or
      (pointA[0]<0 and pointB[0]<0 and pointC[0]<0) or
      (pointA[1]>0 and pointB[1]>0 and pointC[1]>0) or
      (pointA[1]<0 and pointB[1]<0 and pointC[1]<0) or
      (pointA[2]>0 and pointB[2]>0 and pointC[2]>0) or
      (pointA[2]<0 and pointB[2]<0 and pointC[2]<0)) {
    hitDistance = ray2Plane;
  }
  else hitDistance = -1; // Does not intersect triangle
  return hitDistance;
}

void Sphere::printInfo() {
  std::cout <<
    "Object Type : Sphere\n\
    Center: " << center[0] << " " << center[1] << " " << center[2] << "\n\
    Radius: " << radius << "\n\
    Ambient: " << materialProps.ambient[0] << " " << materialProps.ambient[1] << " " << materialProps.ambient[2] << "\n\
    Diffuse: " << materialProps.diffuse[0] << " " << materialProps.diffuse[1] << " " << materialProps.diffuse[2] << "\n\
    Specular: " << materialProps.specular[0] << " " << materialProps.specular[1] << " " << materialProps.specular[2] << "\n\
    Emissive: " << materialProps.emission[0] << " " << materialProps.emission[1] << " " << materialProps.emission[2] << "\n\
    Shininess: " << materialProps.shininess << "\n";
}

float Sphere::hitTest(vec3& eye, vec3& rayDirection) {
  // Define the coefficients of the quadratic equation to be solved.
  float a, b, c, hitDistance;
  float root1, root2, discriminant;
  a = dot(rayDirection, rayDirection);
  b = 2 * dot(rayDirection, eye-center);
  c = dot(eye-center, eye-center) - radius*radius;
  discriminant = b*b - 4*a*c;
  if (discriminant < 0) hitDistance = -1; //no intersection
  else {
    // pick smaller positive root
    root1 = (-b + pow(discriminant, 0.5))/2*a;
    root2 = (-b - pow(discriminant, 0.5))/2*a;

    if (root1 < root2 and root1 > 0) hitDistance = root1;
    else hitDistance = root2;
  }
  return hitDistance;
}
