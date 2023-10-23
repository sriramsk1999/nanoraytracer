#include <vector>
#include <iostream>
#include "SceneObjects.h"

using std::vector, std::pair, std::make_pair, glm::vec3;

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

pair<float, vec3> Triangle::hitTest(vec3& eye, vec3& rayDirection) {
  vec3 hitPoint, pointA, pointB, pointC;
  float ray2Plane, hitDistance=0.;
  auto transformedRay = getTransformedRay(eye, rayDirection);
  vec3 transEye = transformedRay.first;
  vec3 transDirection = transformedRay.second;

  // Find distance between ray and plane
  ray2Plane = ( dot(a, triNorm) - dot(transEye, triNorm) ) / dot( transDirection, triNorm );
  hitPoint = transEye + transDirection*ray2Plane;

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
  return make_pair(hitDistance, hitPoint);
}

vec3 Triangle::getNorm(vec3 hitPoint) {
  return triNorm;
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

pair<float, vec3> Sphere::hitTest(vec3& eye, vec3& rayDirection) {
  auto transformedRay = getTransformedRay(eye, rayDirection);
  vec3 transEye = transformedRay.first;
  vec3 transDirection = transformedRay.second;

  vec3 hitPoint;
  float a, b, c, hitDistance;
  float root1, root2, discriminant;

  // Find the coefficients of the quadratic equation to be solved.
  a = dot(transDirection, transDirection);
  b = 2 * dot(transDirection, transEye-center);
  c = dot(transEye-center, transEye-center) - radius*radius;
  discriminant = b*b - 4*a*c;
  if (discriminant < 0) hitDistance = -1; //no intersection
  else {
    // pick smaller positive root to find first intersection
    root1 = (-b + pow(discriminant, 0.5))/(2*a);
    root2 = (-b - pow(discriminant, 0.5))/(2*a);

    if (root1 < 0 and root2 < 0) hitDistance = -1; //object behind ray
    else {
      if (root1 < root2 and root1 > 0) hitDistance = root1;
      else hitDistance = root2;

      hitPoint = transEye + transDirection*hitDistance;
      hitPoint = vec3(transform * vec4(hitPoint, 1.0));
      hitDistance = length(eye-hitPoint);
    }
  }
  return make_pair(hitDistance, hitPoint);
}

vec3 Sphere::getNorm(vec3 hitPoint) {
  return normalize(hitPoint - center);
}
