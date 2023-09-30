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
    Transform Row1: " << transform[0][0] << " " << transform[1][0] << " " << transform[2][0] << " " << transform[3][0] << "\n\
    Transform Row2: " << transform[0][1] << " " << transform[1][1] << " " << transform[2][1] << " " << transform[3][1] << "\n\
    Transform Row3: " << transform[0][2] << " " << transform[1][2] << " " << transform[2][2] << " " << transform[3][2] << "\n\
    Transform Row4: " << transform[0][3] << " " << transform[1][3] << " " << transform[2][3] << " " << transform[3][3] << "\n\
    Ambient: " << materialProps.ambient[0] << " " << materialProps.ambient[1] << " " << materialProps.ambient[2] << "\n\
    Diffuse: " << materialProps.diffuse[0] << " " << materialProps.diffuse[1] << " " << materialProps.diffuse[2] << "\n\
    Specular: " << materialProps.specular[0] << " " << materialProps.specular[1] << " " << materialProps.specular[2] << "\n\
    Emissive: " << materialProps.emission[0] << " " << materialProps.emission[1] << " " << materialProps.emission[2] << "\n\
    Shininess: " << materialProps.shininess << "\n";
}

float Triangle::hitTest(vec3& eye, vec3& rayDirection) {
  mat4 invTransform = inverse(transform);
  vec3 invEye = vec3(invTransform * vec4(eye, 1.0));
  vec3 invDirection = normalize(vec3(invTransform * vec4(rayDirection, 0.0)));

  vec3 triNorm = normalize( cross(c-a, b-a) );
  vec3 hitPoint, pointA, pointB, pointC;
  float ray2Plane, hitDistance=0.;

  ray2Plane = ( dot(a, triNorm) - dot(invEye, triNorm) ) / dot( invDirection, triNorm );
  hitPoint = invEye + invDirection*ray2Plane;

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
    Transform Row1: " << transform[0][0] << " " << transform[1][0] << " " << transform[2][0] << " " << transform[3][0] << "\n\
    Transform Row2: " << transform[0][1] << " " << transform[1][1] << " " << transform[2][1] << " " << transform[3][1] << "\n\
    Transform Row3: " << transform[0][2] << " " << transform[1][2] << " " << transform[2][2] << " " << transform[3][2] << "\n\
    Transform Row4: " << transform[0][3] << " " << transform[1][3] << " " << transform[2][3] << " " << transform[3][3] << "\n\
    Ambient: " << materialProps.ambient[0] << " " << materialProps.ambient[1] << " " << materialProps.ambient[2] << "\n\
    Diffuse: " << materialProps.diffuse[0] << " " << materialProps.diffuse[1] << " " << materialProps.diffuse[2] << "\n\
    Specular: " << materialProps.specular[0] << " " << materialProps.specular[1] << " " << materialProps.specular[2] << "\n\
    Emissive: " << materialProps.emission[0] << " " << materialProps.emission[1] << " " << materialProps.emission[2] << "\n\
    Shininess: " << materialProps.shininess << "\n";
}

float Sphere::hitTest(vec3& eye, vec3& rayDirection) {
  mat4 invTransform = inverse(transform);
  vec3 invEye = vec3(invTransform * vec4(eye, 1.0));
  vec3 invDirection = normalize(vec3(invTransform * vec4(rayDirection, 0.0)));

  // Define the coefficients of the quadratic equation to be solved.
  vec3 hitPoint;
  float a, b, c, hitDistance;
  float root1, root2, discriminant;

  a = dot(invDirection, invDirection);
  b = 2 * dot(invDirection, invEye-center);
  c = dot(invEye-center, invEye-center) - radius*radius;
  discriminant = b*b - 4*a*c;
  if (discriminant < 0) hitDistance = -1; //no intersection
  else {
    // pick smaller positive root
    root1 = (-b + pow(discriminant, 0.5))/(2*a);
    root2 = (-b - pow(discriminant, 0.5))/(2*a);

    if (root1 < root2 and root1 > 0) hitDistance = root1;
    else hitDistance = root2;

    hitPoint = invEye + invDirection*hitDistance;
    hitPoint = vec3(transform * vec4(hitPoint, 1.0));
    hitDistance = length(eye-hitPoint);
  }
  return hitDistance;
}
